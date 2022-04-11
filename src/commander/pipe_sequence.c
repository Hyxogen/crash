/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipe_sequence.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/25 16:22:25 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/03/28 10:51:19 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"
#include "ft_printf.h"
#include "minishell.h"
#include "memory.h"

#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>


/* for perror */
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>

void
	_cm_close_nostd(int fd)
{
	if (fd != STDIN_FILENO && fd != STDOUT_FILENO && fd != STDERR_FILENO)
		sh_close(fd);
}

static int
	_get_exit_code(int status_code)
{
	if (WIFSIGNALED(status_code))
		return (128 + WTERMSIG(status_code));
	return (WEXITSTATUS(status_code));
}

char **_array_add(char **array, char *value)
{
	size_t	i;

	i = 0;
	while (array[i] != NULL)
		i += 1;
	array = sh_safe_realloc(array, sizeof(*array) * (i + 1), sizeof(*array) * (i + 2));
	array[i] = value;
	array[i + 1] = NULL;
	return (array);
}

char **cm_word_list_to_array(t_minishell *sh, t_snode *word_list)
{
	char	**ret;
	char	**tmp;
	size_t	i;
	size_t	j;

	sh_assert(word_list->type == sx_wordlist);
	ret = sh_safe_malloc(sizeof(*ret));
	ret[0] = NULL;
	i = 0;
	while (i < word_list->childs_size)
	{
		tmp = cm_expand(sh, &word_list->childs[i]->token);
		j = 0;
		while (tmp[j] != NULL)
		{
			ret = _array_add(ret, tmp[j]);
			j += 1;
		}
		free(tmp);
		i += 1;
	}
	return (ret);
}

static t_cm_cmd_proc
	*_get_commandeer_cmd_procs(void)
{
	static t_cm_cmd_proc procs[] = {
		cm_simple_cmd_command,
		cm_if_clause,
		cm_unimplemented_cmd_command,
		cm_case_clause,
		cm_for_clause,
		cm_unimplemented_cmd_command,
		cm_unimplemented_cmd_command
	};

	return (procs);
}

pid_t
	cm_unimplemented_cmd_command(t_minishell *sh, t_snode *node, const int io[3], int closefd)
{
	(void) sh;
	(void) node;
	(void) io;

	(void) closefd;
	ft_fprintf(sh->io[SH_STDERR_INDEX], "%s: Executing this command type is not implemented yet\n", sh->name);
	return (-1);
}

static void
	print_open(void)
{
	int	i;
	int count;
	
	count = 0;
	for (i = 0; i < OPEN_MAX; i++) {
		if (fcntl(i, F_GETFD) < 0) {
			if (errno != EBADF)
				fprintf(stderr, "Error\n");
		} else {
			printf("%d OPEN; ", i);
			count++;
		}
	}
	printf("\n%d/%d open\n", count, OPEN_MAX);
}

static pid_t
	_cm_cmd(t_minishell *sh, t_snode *node, int in, int out)
{
	int				io[3];
	t_cm_cmd_proc	proc;

	io[SH_STDIN_INDEX] = in;
	io[SH_STDOUT_INDEX] = out;
	io[SH_STDERR_INDEX] = STDERR_FILENO;
	proc = _get_commandeer_cmd_procs()[node->type - sx_simple_cmd];
	return (proc(sh, node, io, -1));
}

static int
	_cm_pipe_sequence_wait(pid_t last_process)
{
	pid_t	pid;
	int		return_code;
	int		status_code;

	if (last_process <= 0)
		return_code = cm_convert_retcode(last_process);
	while (1)
	{
		pid = waitpid(0, &status_code, WUNTRACED);
		if (pid < 0)
		{
			sh_assert(errno == ECHILD);
			return (return_code);
		}
		if (pid == last_process)
			return_code = _get_exit_code(status_code);
	}
	sh_assert(0);
	return (-1);
}

/* Will not close begin_in and end_out */
/* TODO make sure that child process also don't close begin_in and end_out */
/* Check fd leak
> echo Hallo | if true; then cat; fi
> ./fdcheck
Will slowly build up more and more fds

> ./fdcheck 1>&2 | ./fdcheck 1>&2 | ./fdcheck
0 OPEN; 1 OPEN; 2 OPEN; 3 OPEN; 12 OPEN; 25 OPEN; 26 OPEN; 
7/10240 open
0 OPEN; 1 OPEN; 2 OPEN; 4 OPEN; 12 OPEN; 25 OPEN; 26 OPEN; 
7/10240 open
0 OPEN; 1 OPEN; 2 OPEN; 12 OPEN; 25 OPEN; 26 OPEN; 
*/
static int
	_cm_pipe_sequence_iter(t_minishell *sh, t_snode *node, const int begin_in, const int end_out)
{
	pid_t			last_process;
	size_t			count;
	size_t			index;
	int				prev_out;
	int				pipe_io[2];

	index = 0;
	count = node->childs_size;
	prev_out = begin_in;
	while (index < count)
	{
		if (index + 1 >= count)
			last_process = _cm_cmd(sh, node->childs[index], prev_out, end_out);
		else
		{
			sh_pipe(pipe_io);
			_cm_cmd(sh, node->childs[index], prev_out, pipe_io[1]);
			if (prev_out != begin_in)
				sh_close(prev_out);
			prev_out = pipe_io[0];
			sh_close(pipe_io[1]);
		}
		index++;
	}
	if (prev_out != begin_in)
		sh_close(prev_out);
	return (_cm_pipe_sequence_wait(last_process));
}

int
	commandeer_pipe_sequence(t_minishell *sh, t_snode *seq_node, const int io[3])
{
	t_pipe_ctx	ctx;
	int			rc;

	if (seq_node->childs_size == 0)
		return (0);
	ctx.pipe_node = seq_node;
	ft_memcpy(ctx.io, io, sizeof(int) * 3);
	cm_disable_reaper(sh);
	rc = _cm_pipe_sequence_iter(sh, seq_node, io[SH_STDIN_INDEX], io[SH_STDOUT_INDEX]);
	cm_enable_reaper(sh);
	return (rc);
}
