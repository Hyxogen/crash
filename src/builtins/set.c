#include "commander.h"
#include "minishell.h"

#include <stdlib.h>

// TODO: better set implementation
// TODO: properly set argv[0]

int
	sh_set(int argc, char **argv)
{
	char	*old_arg0;

	(void) argc;
	old_arg0 = ft_strdup(sh()->args[0]);
	sh_strlst_clear(sh()->args);
	sh()->args = sh_strlst_dup(argv);
	free(sh()->args[0]);
	sh()->args[0] = old_arg0;
	return (0);
}
