/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   return_code.c                                    -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:06 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:06 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"

#include <signal.h>
#include <sys/wait.h>

int
	status_code_to_return_code(int status_code)
{
	if (WIFSIGNALED(status_code))
		return (SH_RETCODE_SIGNALLED_OFFSET + WTERMSIG(status_code));
	return (WEXITSTATUS(status_code));
}

int
	internal_pid_to_return_code(pid_t command_pid)
{
	return (-(command_pid + 1));
}

int
	return_code_to_internal_pid(int return_code)
{
	return (-(return_code + 1));
}
