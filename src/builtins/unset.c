#include "commander.h"
#include "minishell.h"

int
	sh_unset_builtin(int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		sh_unset(argv[i]);
		i += 1;
	}
	return (0);
}
