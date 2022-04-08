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

void
	_do_assignments(t_minishell *sh, t_snode *ass_list, int is_tmp)
{
	size_t	i;
	size_t	j;
	char	*tmp;

	i = 0;
	while (i < ass_list->childs_size)
	{
		j = 0;
		while (ass_list->childs[i]->token.str[j] != '=')
			j += 1;
		tmp = cm_expand_str(sh, &ass_list->childs[i]->token, NULL, ' ');
		// TODO: handle errors (tmp == NULL)
		tmp[j] = '\0';
		// TODO: maybe also errors for readonly stuff?
		// TODO: temporary if there is a command name after
		sh_setenv(sh, tmp, tmp + j + 1, is_tmp);
		free(tmp);
		i += 1;
	}
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
	fprintf(stderr, "Executing this command type is not implemented yet\n");
	return (-1);
}

static int
	_commandeer_pipe_sequence_rec(t_minishell *sh, const t_pipe_ctx *ctx, int prev_out_fd, size_t index)
{
	pid_t			pid;
	int				cmd_io[3];
	int				pipe_io[2];
	t_snode			*cmd_node;
	t_cm_cmd_proc	proc;
	int				ex_code;

	ft_memcpy(cmd_io, ctx->io, sizeof(int) * 3);
	cmd_node = ctx->pipe_node->childs[ctx->pipe_node->childs_size - index];
	cmd_io[SH_STDIN_INDEX] = prev_out_fd;
	proc = _get_commandeer_cmd_procs()[cmd_node->type - sx_simple_cmd];
	if (index != 1)
	{
		sh_pipe(pipe_io);
		cmd_io[SH_STDOUT_INDEX] = pipe_io[1];
		pid = proc(sh, cmd_node, cmd_io, pipe_io[0]); /* TODO make sure pipe_io[0] fd is closed in child */
		if (pid > 0)
			_cm_close_nostd(prev_out_fd);
		_cm_close_nostd(pipe_io[1]);
		ex_code = _commandeer_pipe_sequence_rec(sh, ctx, pipe_io[0], index - 1);
		if (pid > 0)
			sh_waitpid(pid, NULL, 0);
		return (ex_code);
	}
	pid = proc(sh, cmd_node, cmd_io, -1);
	if (pid > 0)
	{
		_cm_close_nostd(cmd_io[SH_STDIN_INDEX]);
		waitpid(pid, &ex_code, 0);
		return (_get_exit_code(ex_code));
	}
	return (-(pid + 1));
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
	rc = _commandeer_pipe_sequence_rec(sh, &ctx, io[STDIN_FILENO], seq_node->childs_size);
	cm_enable_reaper(sh);
	return (rc);
}
