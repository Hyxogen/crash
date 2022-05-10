/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   command_util.c                                   -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:59:23 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:59:23 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

int
	command_should_execute(const t_snode *command)
{
	(void) command;
	if (sh()->continuing)
		return (0);
	return (1);
}

int
	get_argument_count(char *const *argv)
{
	int	argument_count;

	argument_count = 0;
	while (*argv != NULL)
	{
		argv += 1;
		argument_count += 1;
	}
	return (argument_count);
}

char
	**command_get_arguments(const t_snode *command)
{
	return (cm_word_list_to_array(command->childs[0]));
}

void
	handle_execvp_error(const char *name, int error)
{
	if (error == ENOENT)
	{
		sh_err2(name, "command not found");
		exit(127);
	}
	else if (error == EACCES)
	{
		sh_err2(name, "permission denied");
		exit(126);
	}
	else
	{
		sh_err3(name, "unknown error", strerror(errno));
		sh_abort();
	}
}
