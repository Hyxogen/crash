#include "minishell.h"

#include <stdlib.h>

int
	sh_unimplemented(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	sh_err2(argv[0], "not implemented");
	return (EXIT_FAILURE);
}
