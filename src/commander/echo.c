#include "commander.h"

#include <libft.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

static void
	_sh_putstr_fd(const char *str, const int io[3])
{
	if (ft_putstr_fd((char*) str, io[SH_STDOUT_INDEX]) < 0)
	{
		dprintf(io[SH_STDERR_INDEX], "echo(%d): %s\n", errno, strerror(errno)); /* TODO replace with own printf */
		exit(-1); /* TODO: this shouldn't exit the entire shell */
	}
}

int
	sh_echo(t_minishell *sh, char **argv, const int io[3])
{
	const char	*suffix;
	const char	*delim;
	size_t		i;

	(void) sh;
	suffix = "\n";
	delim = "";
	i = 1;
	if (argv[i] != NULL && ft_strcmp(argv[1], "-n") == 0)
	{
		suffix = "";
		i += 1;
	}
	while (argv[i] != NULL)
	{
		_sh_putstr_fd((char *) delim, io);
		_sh_putstr_fd(argv[i], io);
		delim = " ";
		i += 1;
	}
	_sh_putstr_fd((char *) suffix, io);
	return (0);
}
