#include "commander.h"

#include <libft.h>
#include <stdio.h>
#include <stdlib.h>

int
	sh_echo(t_minishell *sh, char **argv, const int io[3])
{
	const char	*suffix;
	const char	*delim;
	size_t		i;

	printf("BUILTIN ECHO\n");
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
		ft_putstr_fd((char *) delim, io[SH_STDOUT_INDEX]);
		ft_putstr_fd(argv[i], io[SH_STDOUT_INDEX]);
		delim = " ";
		i += 1;
	}
	ft_putstr_fd((char *) suffix, io[SH_STDOUT_INDEX]);
	return (-1); // TODO
}
