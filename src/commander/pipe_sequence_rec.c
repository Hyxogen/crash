/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   pipe_sequence_rec.c                              -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:01 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:01 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"

#include <unistd.h>

static int
	execute_commands_recursive_not_last(const t_snode *pipe_seq,
			const int final_out_fd, int previous_out_fd, size_t index)
{
	pid_t	command_pid;
	int		last_return_code;
	int		current_out_fd;

	current_out_fd = create_pipe_and_execute_command(pipe_seq->childs[index],
			&command_pid, previous_out_fd);
	close_nostd_fd(previous_out_fd);
	last_return_code = execute_commands_recursive(pipe_seq,
			final_out_fd, current_out_fd, index + 1);
	wait_and_get_return_code(command_pid);
	return (last_return_code);
}

static int
	execute_commands_recursive_last(const t_snode *pipe_seq,
		const int final_out_fd, int previous_out_fd, size_t index)
{
	pid_t	command_pid;
	int		return_code;
	int		command_io[SH_STDIO_SIZE];

	command_io[SH_STDIN_INDEX] = previous_out_fd;
	command_io[SH_STDOUT_INDEX] = final_out_fd;
	command_io[SH_STDERR_INDEX] = STDERR_FILENO;
	command_pid = execute_command_fork(pipe_seq->childs[index], command_io);
	close_nostd_fd(previous_out_fd);
	return_code = wait_and_get_return_code(command_pid);
	return (return_code);
}

int
	execute_commands_recursive(const t_snode *pipe_seq,
		const int final_out_fd, int previous_out_fd, size_t index)
{
	if (index + 1 == pipe_seq->childs_size)
		return (execute_commands_recursive_last(
				pipe_seq,
				final_out_fd,
				previous_out_fd,
				index));
	else
		return (execute_commands_recursive_not_last(
				pipe_seq,
				final_out_fd,
				previous_out_fd,
				index));
}
