#include "minishell.h"
#include "commander.h"

// TODO: this implementation is incomplete
// TODO: this implementation does not work

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
		if (*str == args[*ind][1])
		{
			buf[0] = *str;
			buf[1] = '\0';
			sh_setenv(key, buf, 0);
			str += 1;
			*ind += 1;
			if (*str != ':')
				return (0);
			// TODO: too few arguments
			sh_setenv("OPTARG", args[*ind], 0);
			str += 1;
			*ind += 1;
			return (0);
		}
		str += 1;
	}
	// TODO: invalid parameter
	return (0);
}

int
	sh_getopts(int argc, char **argv)
{
	char	*optind_str;
	long	optind;
	int		result;

	if (argc < 3)
	{
		sh_err2("usage", "getopts optstring name [arg ...]");
		return (2);
	}
	optind_str = sh_getenv("OPTIND", "1");
	if (sh_atol(optind_str, &optind) < 0)
		optind = 1;
	if (argc == 3)
		result = sh_getopt(argv[1], argv[2], &optind, sh()->args);
	else
		result = sh_getopt(argv[1], argv[2], &optind, argv + 3);
	optind_str = ft_itoa(optind); // TODO: long?
	sh_setenv("OPTIND", optind_str, 0);
	free(optind_str);
	return (result);
}
