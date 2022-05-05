#include "minishell.h"
#include "commander.h"
#include "ft_printf.h"

#include <stdlib.h>

int
	sh_env_builtin(int argc, char **argv)
{
	char	**env;
	size_t	i;

	(void) argc;
	(void) argv;
	env = sh_env();
	i = 0;
	while (env[i] != NULL)
	{
		ft_putstr_fd(env[i], sh()->io[SH_STDOUT_INDEX]);
		ft_putchar_fd('\n', sh()->io[SH_STDOUT_INDEX]);
		i += 1;
	}
	return (0);
}
