#include "minishell.h"

#include "memory.h"
#include <libft.h>

/* Yes, if we don't use 'norminette_uncrasher'
	norminette will actually crash... */
t_envvar
	*sh_setenv_int(const char *key)
{
	t_envvar	*norminette_uncrasher;
	t_envvar	*var;

	norminette_uncrasher = sh_safe_realloc(sh()->vars,
			sizeof(*sh()->vars) * sh()->vars_size,
			sizeof(*sh()->vars) * (sh()->vars_size + 1));
	sh()->vars = norminette_uncrasher;
	var = &sh()->vars[sh()->vars_size];
	sh()->vars_size += 1;
	var->key = ft_strdup(key);
	var->attr = 0;
	var->value = NULL;
	var->tmp_value = NULL;
	return (var);
}

t_envvar
	*sh_getenv_int(const char *key, int create)
{
	size_t	i;

	i = 0;
	while (i < sh()->vars_size)
	{
		if (ft_strcmp(sh()->vars[i].key, key) == 0)
			return (&sh()->vars[i]);
		i += 1;
	}
	if (create)
		return (sh_setenv_int(key));
	return (NULL);
}
