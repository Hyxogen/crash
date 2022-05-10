#include "minishell.h"
#include "commander.h"
#include "memory.h"

#include <stdlib.h>

// ODOT: this implementation is incomplete

static int
	sh_getopt(char *str, char *key, long *ind, char **args)
{
	char	buf[2];

	if (args[*ind] == NULL || args[*ind][0] != '-')
		return (1);
	if (args[*ind][1] == '-')
	{
		*ind += 1;
		return (1);
	}
	while (*str != '\0')
	{
		if (*str == args[*ind][1] && *str != ':')
		{
			buf[0] = *str;
			buf[1] = '\0';
			sh_setenv(key, buf, 0);
			str += 1;
			*ind += 1;
			if (*str != ':')
				return (0);
			if (args[*ind] == NULL)
			{
				sh_setenv(key, "?", 0);
				sh_unset("OPTARG");
				sh_err2("usage", "option requires an argument");
				return (0);
			}
			sh_setenv("OPTARG", args[*ind], 0);
			str += 1;
			*ind += 1;
			return (0);
		}
		str += 1;
	}
	sh_setenv(key, "?", 0);
	sh_unset("OPTARG");
	sh_err2("usage", "illegal option");
	return (0);
}

int
	sh_getopts(int argc, char **argv)
{
	char	*optind_str;
	char	**opt_argv;
	long	optind;
	int		result;
	long	i;

	if (argc < 3)
	{
		sh_err2("usage", "getopts optstring name [arg ...]");
		return (2);
	}
	optind_str = sh_getenv("OPTIND", "1");
	if (sh_atol(optind_str, &i) < 0)
		i = 1;
	opt_argv = argv + 2;
	if (argc == 3)
		opt_argv = sh()->args;
	optind = 0;
	while (optind < i && opt_argv[optind] != NULL)
		optind += 1;
	result = sh_getopt(argv[1], argv[2], &optind, opt_argv);
	optind_str = sh_safe_malloc(21);
	sh_ltoa(optind, optind_str, 21);
	sh_setenv("OPTIND", optind_str, 0);
	return (free(optind_str), result);
}
