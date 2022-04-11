/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   env.c                                            -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:21:50 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:21:51 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "commander.h"
#include "libft.h"
#include "memory.h"
#include <stdlib.h>

t_envvar
	*sh_setenv_int(const char *key)
{
	t_envvar	*var;

	sh()->vars = sh_safe_realloc(sh()->vars,
		sizeof(*sh()->vars) * sh()->vars_size,
		sizeof(*sh()->vars) * (sh()->vars_size + 1));
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

char
	*sh_getenv(const char *key, const char *def)
{
	t_envvar	*var;

	var = sh_getenv_int(key, 0);
	if (var == NULL)
		return ((char*) def);
	if (var->tmp_value != NULL)
		return (var->tmp_value);
	if (var->value == NULL)
		return ((char*) def);
	return (var->value);
}

t_envvar
	*sh_setenv(const char *key, const char *value, int tmp)
{
	t_envvar	*var;

	var = sh_getenv_int(key, 1);
	if (var->attr & SH_ENV_READONLY)
	{
		sh_err2(key, "readonly variable");
		return (NULL);
	}
	if (tmp)
	{
		if (var->tmp_value)
			free(var->tmp_value);
		var->tmp_value = ft_strdup(value);
	}
	else
	{
		if (var->value)
			free(var->value);
		var->value = ft_strdup(value);
	}
	return (var);
}

char
	**sh_env(void)
{
	char	**out;
	size_t	i;
	size_t	j;

	out = sh_safe_malloc(sizeof(*out) * (sh()->vars_size + 1));
	i = 0;
	j = 0;
	while (i < sh()->vars_size)
	{
		if (sh()->vars[i].tmp_value != NULL)
			out[j++] = sh_join2(sh()->vars[i].key, '=', sh()->vars[i].tmp_value);
		else if ((sh()->vars[i].attr & SH_ENV_EXPORT))
			out[j++] = sh_join2(sh()->vars[i].key, '=', sh()->vars[i].value);
		i += 1;
	}
	out[j] = NULL;
	return (out);
}

void
	sh_env_clean(void)
{
	size_t	i;

	i = 0;
	while (i < sh()->vars_size)
	{
		free(sh()->vars[i].tmp_value);
		sh()->vars[i].tmp_value = NULL;
		i += 1;
	}
}

void
	sh_env_init(char **env)
{
	char		*key;
	char		*value;
	size_t		i;
	t_envvar	*var;

	i = 0;
	sh()->vars = NULL;
	sh()->vars_size = 0;
	while (env[i] != NULL)
	{
		sh_split2(env[i], '=', &key, &value);
		var = sh_setenv(key, value, 0);
		var->attr |= SH_ENV_EXPORT;
		free(key);
		free(value);
		i += 1;
	}
}

void
	sh_env_destroy(void)
{
	size_t	i;

	i = 0;
	while (i < sh()->vars_size)
	{
		free(sh()->vars[i].value);
		free(sh()->vars[i].tmp_value);
		free(sh()->vars[i].key);
		i += 1;
	}
	free(sh()->vars);
}
