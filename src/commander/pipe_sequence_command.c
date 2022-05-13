/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   pipe_sequence_command.c                          -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:02:06 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/13 12:37:22 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"

#include <unistd.h>
#include <stdlib.h>

static t_command
	get_command_function(const t_snode *command)
{
	static t_command	functions[] = {
		commandeer_simple_command,
		cm_if_clause,
		cm_function_define,
		cm_case_clause,
		cm_for_clause,
		cm_while_until_clause,
		cm_compound_list,
	};

	return (functions[command->type - sx_simple_cmd]);
}

pid_t
	execute_command_nofork(const t_snode *command, const int io[SH_STDIO_SIZE])
{
	pid_t		command_pid;
	t_command	command_func;

	command_func = get_command_function(command);
	command_pid = command_func(command, io);
	return (command_pid);
}

pid_t
	execute_command_fork(const t_snode *command, const int io[SH_STDIO_SIZE],
		int pipe_io[2])
{
	pid_t		fork_pid;
	pid_t		command_pid;
	int			return_code;

	fork_pid = sh_fork();
	if (fork_pid == 0)
	{
		command_pid = execute_command_nofork(command, io);
		sh_close(pipe_io[0]);
		sh_close(pipe_io[1]);
		return_code = wait_and_get_return_code(command_pid, NULL);
		exit(return_code);
	}
	return (fork_pid);
}

int
	create_pipe_and_execute_command(const t_snode *command, pid_t *command_pid,
			int input_fd)
{
	int	pipe_io[2];
	int	command_io[SH_STDIO_SIZE];

	sh_pipe(pipe_io);
	sh_fdctl(pipe_io[0], SH_FD_FIOCLEX, 1);
	sh_fdctl(pipe_io[1], SH_FD_FIOCLEX, 1);
	command_io[SH_STDIN_INDEX] = input_fd;
	command_io[SH_STDOUT_INDEX] = pipe_io[1];
	command_io[SH_STDERR_INDEX] = STDERR_FILENO;
	*command_pid = execute_command_fork(command, command_io, pipe_io);
	sh_close(pipe_io[1]);
	return (pipe_io[0]);
}
