/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   wait.c                                           -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:09 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:09 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"

int
	command_is_internal(pid_t command_pid)
{
	if (command_pid <= 0)
		return (1);
	return (0);
}

static int
	internal_wait_and_get_return_code(pid_t command_pid)
{
	return (internal_pid_to_return_code(command_pid));
}

int
	process_wait_and_get_return_code(pid_t pid, int *signalled)
{
	int	status_code;

	sh_waitpid(pid, &status_code, WUNTRACED);
	return (status_code_to_return_code(status_code, signalled));
}

int
	wait_and_get_return_code(pid_t command_pid, int *signalled)
{
	if (signalled)
		*signalled = 0;
	if (command_is_internal(command_pid))
		return (internal_wait_and_get_return_code(command_pid));
	return (process_wait_and_get_return_code(command_pid, signalled));
}
