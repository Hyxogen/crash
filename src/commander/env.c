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
#include "ft_printf.h"
#include <stdlib.h>

t_envvar
	*sh_setenv_int(t_minishell *sh, const char *key)
{
	t_envvar	*var;

	sh->vars = sh_safe_realloc(sh->vars,
		sizeof(*sh->vars) * sh->vars_size,
		sizeof(*sh->vars) * (sh->vars_size + 1));
	var = &sh->vars[sh->vars_size];
	sh->vars_size += 1;
	var->key = ft_strdup(key);
	var->attr = 0;
	var->value = NULL;
	var->tmp_value = NULL;
	return (var);
}

t_envvar
	*sh_getenv_int(t_minishell *sh, const char *key, int create)
{
	size_t	i;

	i = 0;
	while (i < sh->vars_size)
	{
		if (ft_strcmp(sh->vars[i].key, key) == 0)
			return (&sh->vars[i]);
		i += 1;
	}
	if (create)
		return (sh_setenv_int(sh, key));
	return (NULL);
}

char
	*sh_getenv(t_minishell *sh, const char *key, const char *def)
{
	t_envvar	*var;

	var = sh_getenv_int(sh, key, 0);
	if (var == NULL)
		return ((char*) def);
	if (var->tmp_value != NULL)
		return (var->tmp_value);
	if (var->value == NULL)
		return ((char*) def);
	return (var->value);
}

t_envvar
	*sh_setenv(t_minishell *sh, const char *key, const char *value, int tmp)
{
	t_envvar	*var;

	var = sh_getenv_int(sh, key, 1);
	if (var->attr & SH_ENV_READONLY)
	{
		ft_fprintf(sh->io[SH_STDERR_INDEX], "%s: %s: readonly variable\n", sh->name, key);
		return (NULL);
	}
	if (tmp)
		var->tmp_value = ft_strdup(value);
	else
		var->value = ft_strdup(value);
	return (var);
}

char
	**sh_env(t_minishell *sh)
{
	char	**out;
	size_t	i;
	size_t	j;

	out = sh_safe_malloc(sizeof(*out) * (sh->vars_size + 1));
	i = 0;
	j = 0;
	while (i < sh->vars_size)
	{
		if (sh->vars[i].tmp_value != NULL)
			out[j++] = sh_join2(sh->vars[i].key, '=', sh->vars[i].tmp_value);
		else if ((sh->vars[i].attr & SH_ENV_EXPORT))
			out[j++] = sh_join2(sh->vars[i].key, '=', sh->vars[i].value);
		i += 1;
	}
	out[j] = NULL;
	return (out);
}

void
	sh_env_clean(t_minishell *sh)
{
	size_t	i;

	i = 0;
	while (i < sh->vars_size)
	{
		free(sh->vars[i].tmp_value);
		sh->vars[i].tmp_value = NULL;
		i += 1;
	}
}

void
	sh_env_init(t_minishell *sh, char **env)
{
	char		*key;
	char		*value;
	size_t		i;
	t_envvar	*var;

	i = 0;
	sh->vars = NULL;
	sh->vars_size = 0;
	while (env[i] != NULL)
	{
		sh_split2(env[i], '=', &key, &value);
		var = sh_setenv(sh, key, value, 0);
		var->attr |= SH_ENV_EXPORT;
		free(key);
		free(value);
		i += 1;
	}
}
