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

char **cm_word_list_to_array(t_snode *word_list)
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
		tmp = cm_expand(&word_list->childs[i]->token);
		j = 0;
		if (tmp == NULL)
		{
			sh_strlst_clear(ret);
			return (NULL);
		}
		while (tmp[j] != NULL)
			ret = _array_add(ret, tmp[j++]);
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
	cm_unimplemented_cmd_command(t_snode *node, const int io[3], int closefd)
{
	(void) node;
	(void) io;

	(void) closefd;
	sh_err1("executing this command type is not implemented yet");
	return (-1);
}

static pid_t
	_cm_cmd(t_snode *node, int in, int out, int pipe_write)
{
	int				io[3];
	t_cm_cmd_proc	proc;

	io[SH_STDIN_INDEX] = in;
	io[SH_STDOUT_INDEX] = out;
	io[SH_STDERR_INDEX] = STDERR_FILENO;
	proc = _get_commandeer_cmd_procs()[node->type - sx_simple_cmd];
	return (proc(node, io, pipe_write));
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

/* TODO make sure that child process also don't close begin_in and end_out */
/* 
> ./fdcheck 1>&2 | ./fdcheck 1>&2 | ./fdcheck
0 OPEN; 1 OPEN; 2 OPEN; 3 OPEN; 12 OPEN; 25 OPEN; 26 OPEN; 
7/10240 open
0 OPEN; 1 OPEN; 2 OPEN; 4 OPEN; 12 OPEN; 25 OPEN; 26 OPEN; 
7/10240 open
0 OPEN; 1 OPEN; 2 OPEN; 12 OPEN; 25 OPEN; 26 OPEN; 
*/
/* Will not close begin_in and end_out */
static int
	_cm_pipe_sequence_iter(t_snode *node, int begin_in, int end_out)
{
	pid_t			last_process;
	size_t			count_index[2];
	int				prev_pipe_io[3];

	count_index[1] = 0;
	count_index[0] = node->childs_size;
	prev_pipe_io[0] = begin_in;
	while (count_index[1] < count_index[0])
	{
		if (count_index[1] + 1 >= count_index[0])
			last_process = _cm_cmd(node->childs[count_index[1]], prev_pipe_io[0], end_out, -1);
		else
		{
			sh_pipe(&prev_pipe_io[1]);
			_cm_cmd(node->childs[count_index[1]], prev_pipe_io[0], prev_pipe_io[2], prev_pipe_io[1]);
			if (prev_pipe_io[0] != begin_in)
				sh_close(prev_pipe_io[0]);
			prev_pipe_io[0] = prev_pipe_io[1];
			sh_close(prev_pipe_io[2]);
		}
		count_index[1]++;
	}
	if (prev_pipe_io[0] != begin_in)
		sh_close(prev_pipe_io[0]);
	return (_cm_pipe_sequence_wait(last_process));
}

int
	commandeer_pipe_sequence(t_snode *seq_node, const int io[3])
{
	int			rc;

	if (seq_node->childs_size == 0)
		return (0);
	cm_disable_reaper();
	rc = _cm_pipe_sequence_iter(seq_node, io[SH_STDIN_INDEX], io[SH_STDOUT_INDEX]);
	cm_enable_reaper();
	return (rc);
}
