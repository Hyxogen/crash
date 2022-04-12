#include "commander.h"
#include "minishell.h"

// TODO: better set implementation
// TODO: properly set argv[0]

int
	sh_set(int argc, char **argv)
{
	(void) argc;
	sh()->args = sh_strlst_dup(argv);
	return (0);
}
