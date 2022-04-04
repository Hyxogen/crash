#include "commander.h"
#include <stdlib.h>


#include <stdio.h>

static char
	**param_expand(t_minishell *sh, t_token *token, size_t i)
{
	char	**result;

	token->parts[0].data = (char*) token->parts[0].data + i;
	result = cm_expand(sh, token, 1);
	token->parts[0].data = (char*) token->parts[0].data - i;
	return (result);
}

static void
	param_error(t_minishell *sh, t_token *token, const char *key, size_t i)
{
	size_t		index;
	char		**exp;
	char		*str;
	(void)		key;

	exp = param_expand(sh, token, i);
	str = sh_strlst_join(exp, ' ');
	index = 0;
	fprintf(stderr, "CraSH: %s: %s\n", key, str);
	sh_strlst_clear(exp);
	free(str);
	if (!sh->interactive)
		exit(EXIT_FAILURE);
	sh_assert(0);
}

static char
	**param_assign(t_minishell *sh, t_token *token, const char *key, size_t i)
{
	char	**str;
	char	*tmp;

	if (key[0] == '*'
		|| key[0] == '@'
		|| key[0] == '#'
		|| key[0] == '?'
		|| key[0] == '-'
		|| key[0] == '$'
		|| key[0] == '!'
		|| ft_isdigit(key[0]))
	{
		// TODO: cannot assign
		return (NULL);
	}
	str = param_expand(sh, token, i);
	tmp = sh_strlst_join(str, ' ');
	sh_strlst_clear(str);
	sh_setenv(sh, key, tmp, 0);
	return (sh_strlst_new(tmp));
}

static char
	**param_get(t_minishell *sh, t_token *token, char *key, size_t i)
{
	char	**var;
	
	var = cm_expand_special(sh, key);
	if (var != NULL
		&& (var[0] == NULL
			|| token->str[i] != ':'
			|| var[0][0] != '\0'))
		return (var);
	sh_strlst_clear(var);
	return (NULL);
}

static char
	**param_subst_null(t_minishell *sh, t_token *token, char *key, size_t i)
{
	if (token->str[i] == '\0')
		return (sh_strlst_new(ft_strdup("")));
	if (token->str[i] == '%' || token->str[i] == '#')
		return (sh_strlst_new(ft_strdup("")));
	i += token->str[i] == ':';
	if (token->str[i] == '+')
		return (sh_strlst_new(ft_strdup("")));
	if (token->str[i] == '-')
		return (param_expand(sh, token, i + 1));
	if (token->str[i] == '?')
		return (param_error(sh, token, key, i + 1), NULL);
	if (token->str[i] == '=')
		return (param_assign(sh, token, key, i + 1));
	return (NULL);
}

static char
	**param_subst_empty_list(t_minishell *sh, t_token *token, char *key, size_t i)
{
	if (token->str[i] == '\0')
		return (sh_strlst_empty());
	if (token->str[i] == '%' || token->str[i] == '#')
		return (sh_strlst_empty());
	i += token->str[i] == ':';
	if (token->str[i] == '+')
		return (sh_strlst_empty());
	if (token->str[i] == '-')
		return (param_expand(sh, token, i + 1));
	if (token->str[i] == '?')
		return (param_error(sh, token, key, i + 1), NULL);
	if (token->str[i] == '=')
		return (param_assign(sh, token, key, i + 1));
	return (NULL);
}

static char
	**param_subst_normal(t_minishell *sh, t_token *token, char *key, size_t i)
{
	char	**str;

	str = param_get(sh, token, key, i);
	if (token->str[i] == '\0')
		return (sh_strlst_dup(str));
	if (token->str[i] == '%' || token->str[i] == '#')
		return (param_pattern(sh, token, str, i));
	i += token->str[i] == ':';
	if (token->str[i] == '-')
		return (sh_strlst_dup(str));
	sh_strlst_clear(str);
	if (token->str[i] == '+')
		return (param_expand(sh, token, i + 1));
	if (token->str[i] == '?')
		return (param_error(sh, token, key, i + 1), NULL);
	if (token->str[i] == '=')
		return (param_assign(sh, token, key, i + 1));
	return (NULL);
}

static char
	**param_subst(t_minishell *sh, t_token *token, char *key, size_t i)
{
	char	**str;
	char	**tmp;

	str = param_get(sh, token, key, i);
	if (str == NULL)
		tmp = param_subst_null(sh, token, key, i);
	else if (str[0] == NULL)
		tmp = param_subst_empty_list(sh, token, key, i);
	else if (str[0][0] == '\0')
		tmp = param_subst_null(sh, token, key, i);
	else
		tmp = param_subst_normal(sh, token, key, i);
	sh_strlst_clear(str);
	return (tmp);
}

size_t
	param_length(const char *param)
{
	size_t	i;

	if (param[0] == '*'
		|| param[0] == '@'
		|| param[0] == '#'
		|| param[0] == '?'
		|| param[0] == '-'
		|| param[0] == '$'
		|| param[0] == '!')
		return (1);
	i = 0;
	if (ft_isdigit(param[0]))
		while (ft_isdigit(param[i]))
			i += 1;
	else
		while (ft_isalnum(param[i]) || param[i] == '_')
			i += 1;
	return (i);
}

char
	**cm_expand_param(t_minishell *sh, t_token *token)
{
	char	**var;
	char	*key;
	size_t	i;
	char	**str;

	if (token->str[0] == '#' && (token->str[1] != '#'
		|| token->str[2] == '\0'))
	{
		i = param_length(token->str + 1) + 1;
		if (token->str[i] != '\0')
			return (NULL);
		var = cm_expand_special(sh, token->str + 1);
		if (var == NULL || var[0] == NULL || var[1] != NULL)
			return (sh_strlst_new(ft_strdup("0")));
		return (sh_strlst_new(ft_itoa(ft_strlen(var[0]))));
	}
	i = param_length(token->str);
	key = ft_strdup(token->str);
	key[i] = '\0';
	str = param_subst(sh, token, key, i);
	free(key);
	return (str);
}
