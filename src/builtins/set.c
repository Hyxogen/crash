#include "commander.h"
#include "minishell.h"

#include <stdlib.h>

// TODO: better set implementation
// TODO: properly set argv[0]

int
	sh_set(int argc, char **argv)
{
	char	*old_arg0;
	char	**current;

	(void) argc;
	current = argv;
	while (*current)
	{
		if ((*current)[0] == '-')
		{
			sh_err3(sh()->name, argv[0],
				"warning options are ignored for this command");
			break ;
		}
		current += 1;
	}
	old_arg0 = ft_strdup(sh()->args[0]);
	sh_strlst_clear(sh()->args);
	sh()->args = sh_strlst_dup(argv);
	free(sh()->args[0]);
	sh()->args[0] = old_arg0;
	return (0);
}
