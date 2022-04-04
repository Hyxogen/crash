#include "commander.h"

#include <libft.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

static int
	_sh_putstr_fd(const char *str, const int io[3])
{
	if (ft_putstr_fd((char*) str, io[SH_STDOUT_INDEX]) < 0)
	{
		dprintf(io[SH_STDERR_INDEX], "echo(%d): %s\n", errno, strerror(errno)); /* TODO replace with own printf */
		return (-1);
	}
	return (0);
}

int
	sh_echo(t_minishell *sh, int argc, char **argv, const int io[3])
{
	const char	*suffix;
	const char	*delim;
	size_t		i;

	(void) sh;
	(void) argc;
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
		if (_sh_putstr_fd((char *) delim, io) || _sh_putstr_fd(argv[i], io))
			return (-1);
		delim = " ";
		i += 1;
	}
	if (_sh_putstr_fd((char *) suffix, io))
		return (-1);
	return (0);
}
