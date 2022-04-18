#include "minishell.h"

#include <stdlib.h>

static void
	_cm_write_here(int fd, const char *str, int skip_leading_tabs)
{
	const char	*next_newline;
	size_t		length;

	if (!skip_leading_tabs)
	{
		sh_write(fd, str, ft_strlen(str));
		return ;
	}
	while (*str)
	{
		while (*str == '\t')
			str++;
		next_newline = ft_strchr(str, '\n');
		if (!next_newline)
		{
			sh_write(fd, str, ft_strlen(str));
			break ;
		}
		sh_write(fd, str, (next_newline - str) + 1);
		str = next_newline + 1;
	}
	return ;
}

int
	_cm_create_and_write_here(const char *str, int skip_leading_tabs)
{
	int		here_pipe[2];
	pid_t	writer_pid;

	sh_pipe(here_pipe);
	writer_pid = sh_fork();
	if (writer_pid == 0)
	{
		sh_close(here_pipe[0]);
		_cm_write_here(here_pipe[1], str, skip_leading_tabs);
		sh_close(here_pipe[1]);
		exit(EXIT_SUCCESS);
	}
	sh_close(here_pipe[1]);
	return (here_pipe[0]);
}
