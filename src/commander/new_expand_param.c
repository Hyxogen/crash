#include "commander.h"
#include "minishell.h"
#include "memory.h"

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

size_t
	expand_length(t_expand *exp)
{
	size_t	i;

	sh_assert(exp->count == 1);
	i = 0;
	if (exp->parts[0].array)
		while (exp->parts[0].str[i] != NULL)
			i += 1;
	else
		while (exp->parts[0].str[0][i] != '\0')
			i += 1;
	return (i);
}

int
	expand_special(t_minishell *sh, t_expand *exp, char *key)
{
	char	*result;

	result = sh_getenv(sh, key, NULL);
	if (result == NULL)
		return (-1);
	expansion_add_part(exp, sh_strlst_new(ft_strdup(result)), 0);
	return (0);
}

int
	param_subst(t_minishell *sh, t_expand *exp, t_param_ctx *ctx)
{
	// TODO: implement
	return (expand_special(sh, exp, ctx->key));
}

int
	expand_param(t_minishell *sh, t_expand *exp, t_token *token)
{
	t_expand	tmp;
	t_param_ctx	ctx;
	int			result;
	size_t		i;

	ctx.token = token;
	ctx.i = param_length(token->str);
	if (token->str[0] == '#' && (token->str[1] != '#'
		|| token->str[2] == '\0'))
	{
		if (token->str[ctx.i] != '\0')
			return (-1);
		expansion_init(&tmp);
		expand_special(sh, &tmp, token->str + 1);
		expansion_add_part(exp, sh_strlst_new(ft_itoa(expand_length(&tmp))), 0);
		expansion_destroy(&tmp);
		return (0);
	}
	expansion_init(&tmp);
	ctx.key = ft_strdup(token->str);
	ctx.key[ctx.i] = '\0';
	result = param_subst(sh, &tmp, &ctx);
	free(ctx.key);
	i = 0;
	while (i < tmp.count)
	{
		expansion_add_part(exp, sh_strlst_dup(tmp.parts[i].str), tmp.parts[i].quote);
		i += 1;
	}
	expansion_destroy(&tmp);
	return (result);
}