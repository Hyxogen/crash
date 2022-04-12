#include "minishell.h"

#include <limits.h>

int
	sh_continue(int argc, char **argv)
{
	long	count;

	if (!sh()->loop_depth)
	{
		sh_err2("continue", "only meaningful in a `for', `while', or `until' loop");
		return (0);	
	}
	if (argc <= 1)
		count = 1;
	else if (argc >= 3)
	{
		sh_err2("continue", "too many arguments");
		sh()->breaking = sh()->loop_depth; /* TODO this should probably be handled differently (also check break.c)  */
		return (1);
	}
	else if (sh_atol(argv[1], &count) < 0)
	{
		sh_err2("continue", "numeric argument required");
		sh()->breaking = INT_MAX;
		return (1);
	}
	if (count > sh()->loop_depth)
		count = sh()->loop_depth;
	sh()->continuing = 1;
	if (count > 1)
		sh()->breaking = count - 1;
	return (0);
}
