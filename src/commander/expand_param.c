#include "commander.h"
#include "minishell.h"
#include "memory.h"
#include <stdlib.h>
#include <unistd.h>

size_t
	expand_key_length(const char *param)
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

int
	expand_empty(t_expand *exp, int empty_is_null)
{
	if (exp->count == 0 || exp->parts[0].str[0] == NULL)
		return (1);
	if (exp->parts[0].str[0][0] == '\0'
		&& exp->parts[0].str[1] == NULL
		&& empty_is_null)
		return (1);
	return (0);
}

size_t
	expand_length(t_expand *exp)
{
	size_t	i;

	if (exp->count == 0)
		return (0);
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
	expand_special(t_expand *exp, char *key)
{
	char	*result;

	// TODO: special parameters
	if (key[0] == '@')
	{
		expansion_add_part(exp, sh_strlst_dup(sh()->args + 1), 0);
		exp->parts[exp->count - 1].array = 1;
		return (0);
	}
	result = sh_getenv(key, NULL);
	if (result == NULL)
		return (-1);
	expansion_add_part(exp, sh_strlst_new(ft_strdup(result)), 0);
	return (0);
}

int
	expand_error(t_param_ctx *ctx, size_t i)
{
	char	*str;

	if (((char *) ctx->token->parts[0].data)[i] == '\0')
		sh_err2(ctx->key, "parameter not set");
	else
	{
		ctx->token->parts[0].data = (char*) ctx->token->parts[0].data + i;
		str = cm_expand_str(ctx->token, NULL, ' ');
		ctx->token->parts[0].data = (char*) ctx->token->parts[0].data - i;
		sh_err2(ctx->key, str);
		free(str);
	}
	if (!sh()->interactive)
		exit(EXIT_FAILURE);
	return (-1);
}

int
	expand_assign(t_expand *exp, t_param_ctx *ctx, size_t i)
{
	char	*str;

	if (ctx->key[0] == '*'
		|| ctx->key[0] == '@'
		|| ctx->key[0] == '#'
		|| ctx->key[0] == '?'
		|| ctx->key[0] == '-'
		|| ctx->key[0] == '$'
		|| ctx->key[0] == '!'
		|| ft_isdigit(ctx->key[0]))
	{
		sh_err2(ctx->key, "cannot assign in this way");
		return (-1);
	}
	ctx->token->parts[0].data = (char*) ctx->token->parts[0].data + i;
	str = cm_expand_str(ctx->token, NULL, ' ');
	ctx->token->parts[0].data = (char*) ctx->token->parts[0].data - i;
	sh_setenv(ctx->key, str, 0);
	expansion_add_part(exp, sh_strlst_new(str), 0);
	return (0);
}

int
	expand_right(t_expand *exp, t_param_ctx *ctx, size_t i)
{
	t_expand	tmp;
	int			result;

	ctx->token->parts[0].data = (char*) ctx->token->parts[0].data + i;
	result = cm_expand_list(&tmp, ctx->token);
	ctx->token->parts[0].data = (char*) ctx->token->parts[0].data - i;
	expansion_copy_parts(exp, &tmp);
	expansion_destroy(&tmp);
	return (result);
}

int
	expand_pattern(t_expand *exp,
		t_param_ctx *ctx, t_expand *tmp)
{
	(void) sh;
	(void) exp;
	(void) ctx;
	(void) tmp;
	// TODO: implement
	return (-1);
}

int
	expand_promote(t_expand *exp, t_expand *tmp)
{
	if (tmp->count == 0)
		expansion_add_part(exp, sh_strlst_new(ft_strdup("")), 0);
	else
		expansion_copy_parts(exp, tmp);
	expansion_destroy(tmp);
	return (0);
}

int
	expand_subst(t_expand *exp, t_param_ctx *ctx)
{
	t_expand	tmp;
	int			empty;

	expansion_init(&tmp);
	expand_special(&tmp, ctx->key);
	if (ctx->token->str[ctx->i] == '\0')
		return (expand_promote(exp, &tmp));
	if (ctx->token->str[ctx->i] == '%' || ctx->token->str[ctx->i] == '#')
		return (expand_pattern(exp, ctx, &tmp));
	empty = expand_empty(&tmp, ctx->token->str[ctx->i] == ':');
	ctx->i += ctx->token->str[ctx->i] == ':';
	if (ctx->token->str[ctx->i] == '+' && empty
		&& tmp.count == 1 && tmp.parts[0].array)
		return (expansion_add_part(exp, sh_strlst_empty(), 0), 0);
	if ((ctx->token->str[ctx->i] == '-' && !empty)
		|| (ctx->token->str[ctx->i] == '+' && empty))
		return (expand_promote(exp, &tmp));
	expansion_destroy(&tmp);
	if (ctx->token->str[ctx->i] == '+' || ctx->token->str[ctx->i] == '-')
		return (expand_right(exp, ctx, ctx->i + 1));
	if (ctx->token->str[ctx->i] == '?')
		return (expand_error(ctx, ctx->i + 1));
	if (ctx->token->str[ctx->i] == '=')
		return (expand_assign(exp, ctx, ctx->i + 1));
	return (-1);
}

int
	expand_param(t_expand *exp, t_token *token)
{
	t_expand	tmp;
	t_param_ctx	ctx;
	int			result;

	ctx.token = token;
	if (token->str[0] == '#' && (token->str[1] != '#'
		|| token->str[2] == '\0'))
	{
		ctx.i = expand_key_length(token->str + 1) + 1;
		if (token->str[ctx.i] != '\0')
			return (-1);
		expansion_init(&tmp);
		expand_special(&tmp, token->str + 1);
		expansion_add_part(exp,
			sh_strlst_new(ft_itoa(expand_length(&tmp))), 0);
		expansion_destroy(&tmp);
		return (0);
	}
	ctx.i = expand_key_length(token->str);
	ctx.key = ft_strdup(token->str);
	ctx.key[ctx.i] = '\0';
	result = expand_subst(exp, &ctx);
	free(ctx.key);
	return (result);
}
