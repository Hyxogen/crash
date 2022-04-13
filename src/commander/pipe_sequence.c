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

int
	_cm_convert_builtin_pid(pid_t pid)
{
	return (-(pid + 1));
}

static int
	_cm_wait_cmd(pid_t pid)
{
	int	status;

	if (pid <= 0)
		return (_cm_convert_builtin_pid(pid));
	sh_waitpid(pid, &status, WUNTRACED);
	return (_get_exit_code(status));	
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

/* TODO implement subshells */
static t_cm_cmd_proc
	*_get_commandeer_cmd_procs(void)
{
	static t_cm_cmd_proc procs[] = {
		cm_simple_cmd_command,
		cm_if_clause,
		cm_function_define,
		cm_case_clause,
		cm_for_clause,
		cm_while_until_clause
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
	_cm_cmd_nofork(t_snode *node, int in, int out, int pipe_write)
{
	int				io[3];
	t_cm_cmd_proc	proc;

	io[SH_STDIN_INDEX] = in;
	io[SH_STDOUT_INDEX] = out;
	io[SH_STDERR_INDEX] = STDERR_FILENO;
	proc = _get_commandeer_cmd_procs()[node->type - sx_simple_cmd];
	return (proc(node, io, pipe_write));
}

static pid_t
	_cm_cmd_fork(t_snode *node, int in, int out, int pipe_write)
{
	int				io[3];
	t_cm_cmd_proc	proc;
	pid_t			pid;

	pid = sh_fork();
	if (pid == 0)
	{
		io[SH_STDIN_INDEX] = in;
		io[SH_STDOUT_INDEX] = out;
		io[SH_STDERR_INDEX] = STDERR_FILENO;
		proc = _get_commandeer_cmd_procs()[node->type - sx_simple_cmd];
		exit(_cm_wait_cmd(proc(node, io, pipe_write)));
	}
	return (pid);
}

static int
	_cm_pipe_sequence_rec(t_pipe_ctx *ctx, int prev_out, size_t index)
{
	int		ret_code;
	int		pipe_io[2];
	pid_t	pid;

	if (index + 1 >= ctx->node->childs_size)
	{
		pid = _cm_cmd_fork(ctx->node->childs[index], prev_out, ctx->end_out, -1);
		if (prev_out != ctx->begin_in)
			sh_close(prev_out);
		return (_cm_wait_cmd(pid));
	}
	sh_pipe(pipe_io);
	pid = _cm_cmd_fork(ctx->node->childs[index], prev_out, pipe_io[1], pipe_io[0]);
	if (prev_out != ctx->begin_in)
		sh_close(prev_out);
	sh_close(pipe_io[1]);
	ret_code = _cm_pipe_sequence_rec(ctx, pipe_io[0], index + 1);
	_cm_wait_cmd(pid);
	return (ret_code);
}

int
	commandeer_pipe_sequence(t_snode *seq_node, const int io[3])
{
	t_pipe_ctx	ctx;
	int			rc;

	if (seq_node->childs_size == 0 || sh()->breaking > 0)
		return (0);
	else if (seq_node->childs_size == 1)
	{
		cm_disable_reaper();
		rc = _cm_wait_cmd(_cm_cmd_nofork(seq_node->childs[0], io[SH_STDIN_INDEX], io[SH_STDOUT_INDEX], -1));
		cm_enable_reaper();
		return (rc);
	}
	ctx.begin_in = io[SH_STDIN_INDEX];
	ctx.end_out = io[SH_STDOUT_INDEX];
	ctx.node = seq_node;
	cm_disable_reaper();
	rc = _cm_pipe_sequence_rec(&ctx, io[SH_STDIN_INDEX], 0);
	cm_enable_reaper();
	sh()->return_code = rc; /* TODO check if this should be here */
	return (rc);
}
