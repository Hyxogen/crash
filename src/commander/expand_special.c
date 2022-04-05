#include "minishell.h"
#include "commander.h"

char
	**cm_expand_special(t_minishell *sh, const char *key)
{
	char	*result;

	if (key[0] == '@')
		return (sh_strlst_dup(sh->args + 1));
	result = sh_getenv(sh, key, NULL);
	if (result == NULL)
		return (NULL);
	return (sh_strlst_new(ft_strdup(result)));
}
