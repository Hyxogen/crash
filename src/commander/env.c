#include "minishell.h"
#include "libft.h"
#include "memory.h"
#include <stdlib.h>

t_envvar
	*sh_getenv(t_minishell *sh, const char *key)
{
	size_t	i;

	i = 0;
	while (i < sh->vars_size)
	{
		if (ft_strcmp(sh->vars[i].key, key) == 0)
			return (&sh->vars[i]);
		i += 1;
	}
	return (NULL);
}

/* TODO: check readonly attribute */
t_envvar
	*sh_setenv(t_minishell *sh, char *key, char *value)
{
	t_envvar	*env;

	env = sh_getenv(sh, key);
	if (env != NULL)
	{
		free(env->value);
		env->value = value;
	}
	else
	{
		sh->vars = sh_safe_realloc(sh->vars,
			sizeof(*sh->vars) * sh->vars_size,
			sizeof(*sh->vars) * (sh->vars_size + 1));
		env = &sh->vars[sh->vars_size];
		sh->vars_size += 1;
		env->key = key;
		env->value = value;
	}
	env->attr = 0;
	return (env);
}

/* TODO: check export attribute */
char
	**sh_env(t_minishell *sh)
{
	char	**result;
	size_t	i;

	result = sh_safe_malloc(sizeof(*result) * (sh->vars_size + 1));
	i = 0;
	while (i < sh->vars_size)
	{
		result[i] = sh_join2(sh->vars[i].key, '=', sh->vars[i].value);
		i += 1;
	}
	result[i] = NULL;
	return (result);
}

/* TODO: set export attribute? */
void
	sh_env_init(t_minishell *sh, char **env)
{
	char	*key;
	char	*value;
	size_t	i;

	i = 0;
	sh->vars = NULL;
	sh->vars_size = 0;
	while (env[i] != NULL)
	{
		sh_split2(env[i], '=', &key, &value);
		sh_setenv(sh, key, value);
		i += 1;
	}
}
