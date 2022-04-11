#include "commander.h"

#include <libft.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

static int
	_sh_putstr_fd(const char *str)
{
	if (ft_putstr_fd((char*) str, sh()->io[SH_STDOUT_INDEX]) < 0)
	{
		sh_err2("echo", strerror(errno));
		return (-1);
	}
	return (0);
}

int
	sh_echo(int argc, char **argv)
{
	const char	*suffix;
	const char	*delim;
	size_t		i;

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
		if (_sh_putstr_fd((char *) delim) || _sh_putstr_fd(argv[i]))
			return (-1);
		delim = " ";
		i += 1;
	}
	if (_sh_putstr_fd((char *) suffix))
		return (-1);
	return (0);
}
