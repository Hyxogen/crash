#include "commander.h"
#include "minishell.h"

#include <libft.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int
	sh_exit(int argc, char **argv)
{
	long	ex_code;

	if (argc <= 1)
		exit(0);
	if (argc >= 3)
	{
		sh_err2("exit", "too many arguments");
		exit(0);
	}
	if (sh_atol(argv[1], &ex_code) < 0)
	{
		sh_err3("exit", argv[1], "numeric argument required");
		exit(0);
	}
	exit(ex_code);
	return (-1);
}
