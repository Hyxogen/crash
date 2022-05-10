/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   pipe_sequence.c                                  -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:59:52 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:59:52 by csteenvo            :      ..            */
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

static int
	pipe_seq_should_execute(const t_snode *pipe_seq)
{
	if (pipe_seq->childs_size == 0 || sh()->breaking > 0)
		return (0);
	return (1);
}

static int
	execute_pipe_seq_nofork(const t_snode *pipe_seq,
		const int io[SH_STDIO_SIZE])
{
	return (execute_commands_recursive(
			pipe_seq,
			io[SH_STDOUT_INDEX],
			io[SH_STDIN_INDEX],
			0));
}

static int
	execute_multi_command_pipe_seq(const t_snode *pipe_seq,
		const int io[SH_STDIO_SIZE])
{
	pid_t	pipe_pid;
	int		return_code;

	pipe_pid = sh_fork();
	if (pipe_pid == 0)
	{
		return_code = execute_pipe_seq_nofork(pipe_seq, io);
		exit(return_code);
	}
	return_code = process_wait_and_get_return_code(pipe_pid);
	return (return_code);
}

static int
	execute_single_command_pipe_seq(const t_snode *pipe_seq,
		const int io[SH_STDIO_SIZE])
{
	pid_t	command_pid;
	int		return_code;

	command_pid = execute_command_nofork(pipe_seq->childs[0], io);
	return_code = wait_and_get_return_code(command_pid);
	return (return_code);
}

int
	execute_pipe_seq(const t_snode *pipe_seq,
		const int io[SH_STDIO_SIZE])
{
	int	return_code;

	if (!pipe_seq_should_execute(pipe_seq))
		return (0);
	if (pipe_seq->childs_size == 1)
		return_code = execute_single_command_pipe_seq(pipe_seq, io);
	else
		return_code = execute_multi_command_pipe_seq(pipe_seq, io);
	sh()->return_code = return_code;
	return (return_code);
}
