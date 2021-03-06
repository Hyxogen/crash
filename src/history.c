/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   history.c                                        -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:12 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:12 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <libft.h>
#include <stdlib.h>

void
	history_append(const char *line)
{
	char	*last_command;
	char	*norminette_satisfier;

	last_command = sh()->last_command;
	if (last_command == NULL)
	{
		sh()->last_command = ft_strdup(line);
	}
	else
	{
		norminette_satisfier = sh_join2(last_command, '\n', line);
		sh()->last_command = norminette_satisfier;
		free(last_command);
	}
}

const char
	*history_get_last_command(void)
{
	return (sh()->last_command);
}

void
	history_new_command(void)
{
	free(sh()->last_command);
	sh()->last_command = NULL;
}
