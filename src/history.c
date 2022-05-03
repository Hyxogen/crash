#include "minishell.h"

#include <libft.h>
#include <stdlib.h>

void
	history_append(const char *line)
{
	char	*last_command;

	last_command = sh()->last_command;
	if (last_command == NULL)
	{
		sh()->last_command = ft_strdup(line);
	}
	else
	{
		sh()->last_command = sh_join2(last_command, '\n', line);
		free(last_command);
	}
}

const char
	*history_get_last(void)
{
	return (sh()->last_command);
}

void
	history_clear(void)
{
	free(sh()->last_command);
	sh()->last_command = NULL;
}
