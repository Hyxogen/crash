#include "minishell.h"
#include "commander.h"
#include "ft_printf.h"

#include <stdlib.h>

int
	sh_pwd(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	ft_fprintf(sh()->io[SH_STDOUT_INDEX], "%s\n", sh()->pwd);
	return (0);
}
