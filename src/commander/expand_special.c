#include "minishell.h"
#include "commander.h"

char
	**cm_expand_special(t_minishell *sh, const char *key)
{
	if (key[0] == '@')
		return (sh_strlst_dup(sh->args + 1));
	return (sh_strlst_new(sh_getenv(sh, key, NULL)));
}
