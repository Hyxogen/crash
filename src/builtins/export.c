#include "minishell.h"
#include "commander.h"

// TODO: better export implementation

int
	sh_export(int argc, char **argv)
{
	t_envvar	*var;

	(void) argc;
	var = sh_getenv_int(argv[1], 1);
	var->attr |= SH_ENV_EXPORT;
	return (0);
}
