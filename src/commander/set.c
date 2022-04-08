#include "commander.h"

int
	sh_set(t_minishell *sh, int argc, char **argv, const int io[3])
{
	sh->args = argv;
	return (0);
}
