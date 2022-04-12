#include "minishell.h"

#include <limits.h>

int
	sh_break(int argc, char **argv)
{
	long	count;

	if (!sh()->loop_depth)
	{
		sh_err2("break", "only meaningful in a `for', `while', or `until' loop");
		return (0);	
	}
	if (argc <= 1)
		count = 1;
	else if (argc >= 3)
	{
		sh_err2("break", "too many arguments");
		sh()->breaking = INT_MAX;
		return (1);
	}
	else if (sh_atol(argv[1], &count) < 0)
	{
		sh_err2("break", "numeric argument required");
		sh()->breaking = INT_MAX;
		return (1);
	}
	sh()->breaking = count;
	if (count > sh()->loop_depth)
		sh()->breaking = sh()->loop_depth;
	printf("I should be breaking\n");
	return (0);
}
