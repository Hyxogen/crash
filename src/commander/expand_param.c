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
	char		**exp;
	char		*str;
	(void)		key;

	exp = param_expand(sh, token, i);
	str = sh_strlst_join(exp, ' ');
	fprintf(stderr, "CraSH: %s: %s\n", key, str); // TODO: not use fprintf
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

static int
	param_empty(char **var, int empty_is_null)
{
	if (var == NULL || var[0] == NULL)
		return (1);
	if (var[0][0] == '\0' && empty_is_null)
		return (1);
	return (0);
}

static char
	**param_promote(char **str)
{
	if (str == NULL)
		return (sh_strlst_new(ft_strdup("")));
	return (str);
}

static char
	**param_subst(t_minishell *sh, t_token *token, char *key, size_t i)
{
	char	**str;
	int		empty;

	str = cm_expand_special(sh, key);
	if (token->str[i] == '\0')
		return (param_promote(str));
	if (token->str[i] == '%' || token->str[i] == '#')
		return (param_pattern(sh, token, str, i));
	empty = param_empty(str, token->str[i] == ':');
	i += token->str[i] == ':';
	if ((token->str[i] == '-' && !empty)
		|| (token->str[i] == '+' && empty))
		return (param_promote(str));
	sh_strlst_clear(str);
	if (token->str[i] == '+' || token->str[i] == '-')
		return (param_expand(sh, token, i + 1));
	if (token->str[i] == '?')
		return (param_error(sh, token, key, i + 1), NULL);
	if (token->str[i] == '=')
		return (param_assign(sh, token, key, i + 1));
	return (NULL);
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
