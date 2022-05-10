#include "minishell.h"

#include <limits.h>

/* 22: ODOT: this should probably be handled differently (PS: break.c) */

int
	sh_continue(int argc, char **argv)
{
	long	count;

	if (!sh()->loop_depth)
	{
		return (sh_err2("continue",
				"only meaningful in a `for', `while', or `until' loop"), 0);
	}
	if (argc <= 1)
		count = 1;
	else if (argc >= 3)
	{
		sh_err2("continue", "too many arguments");
		return (sh()->breaking = sh()->loop_depth, 1);
	}
	else if (sh_atol(argv[1], &count) < 0)
	{
		sh_err3("continue", argv[1], "numeric argument required");
		return (sh()->breaking = INT_MAX, 1);
	}
	if (count > sh()->loop_depth)
		count = sh()->loop_depth;
	sh()->continuing = 1;
	if (count > 1)
		sh()->breaking = count - 1;
	return (0);
}
