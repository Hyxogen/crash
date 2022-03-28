#include "commander.h"
#include <stdlib.h>

static char
	*param_expand(t_minishell *sh, t_token *token, size_t i)
{
	char	**tmp;
	char	*str;

	tmp = cm_expand(sh, token);
	if (tmp == NULL)
		return (NULL);
	// TODO: what happens if fields are split?
	str = ft_strdup(tmp[0] + i);
	free(tmp[0]);
	free(tmp);
	return (str);
}

static char
	*param_pattern(t_minishell *sh, t_token *token, const char *str, size_t i)
{
	(void) sh;
	(void) token;
	(void) str;
	(void) i;
	// TODO: implement
	sh_assert(0);
}

static char
	*param_error(t_minishell *sh, t_token *token, const char *key, size_t i)
{
	(void) sh;
	(void) token;
	(void) key;
	(void) i;
	// TODO: implement
	sh_assert(0);
}

static char
	*param_subst(t_minishell *sh, t_token *token, char *key, size_t i)
{
	t_envvar	*var;
	char		*str;

	var = sh_getenv(sh, key);
	str = NULL;
	if (var != NULL && (token->str[i] != ':' || var->value[0] != '\0'))
		str = var->value;
	if (token->str[i] == '\0' && str != NULL)
		return (ft_strdup(str));
	if (token->str[i] == '\0')
		return (ft_strdup(""));
	i += token->str[i] == ':';
	if (token->str[i] == '%' || token->str[i] == '#')
		return (param_pattern(sh, token, str, i));
	if (token->str[i] == '+' && str != NULL)
		return (param_expand(sh, token, i + 1));
	if (token->str[i] == '+')
		return (ft_strdup(""));
	if (token->str[i] != '-' && token->str[i] != '=' && token->str[i] != '?')
		return (NULL);
	if (str != NULL)
		return (ft_strdup(str));
	if (token->str[i] == '-')
		return (param_expand(sh, token, i + 1));
	if (token->str[i] == '?')
		return (param_error(sh, token, key, i + 1));
	str = param_expand(sh, token, i + 1);
	sh_setenv(sh, ft_strdup(key), ft_strdup(str));
	return (str);
}

char
	*cm_expand_param(t_minishell *sh, t_token *token)
{
	t_envvar	*var;
	size_t		i;
	char		*key;
	char		*str;

	i = token->str[0] == '#';
	while (ft_isalnum(token->str[i]) || token->str[i] == '_')
		i += 1;
	if (token->str[0] == '#')
	{
		if (token->str[i] != '\0')
			return (NULL);
		var = sh_getenv(sh, token->str + 1);
		if (var == NULL)
			return (ft_strdup("0"));
		return ft_itoa(ft_strlen(var->value));
	}
	key = ft_strdup(token->str);
	key[i] = '\0';
	str = param_subst(sh, token, key, i);
	free(key);
	return (str);
}
