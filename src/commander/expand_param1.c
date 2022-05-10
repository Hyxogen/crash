#include "commander.h"

#include "memory.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

void
	expand_pattern_do(t_exp_param_ctx *ctxx, t_expand *exp, t_param_ctx *ctx)
{
	while (exp->parts[exp->count - 1].str[ctxx->i] != NULL)
	{
		ctxx->j = 0;
		ctxx->str = exp->parts[exp->count - 1].str[ctxx->i];
		if (ctx->token->str[ctx->i] == '%')
			expand_pattern_percent(ctxx);
		else
			expand_pattern_hash(ctxx, exp);
		ctxx->i += 1;
	}
}

int
	expand_pattern(t_expand *exp, t_param_ctx *ctx, t_expand *tmp)
{
	t_exp_param_ctx	ctxx;

	ctxx.long_mode = ctx->token->str[ctx->i] == ctx->token->str[ctx->i + 1];
	ctxx.info = NULL;
	ctx->token->parts[0].data = (char *) ctx->token->parts[0].data
		+ ctx->i + ctxx.long_mode + 1;
	ctxx.pattern = cm_expand_str(ctx->token, &ctxx.info, ' ', 1);
	ctx->token->parts[0].data = (char *) ctx->token->parts[0].data
		- ctx->i - ctxx.long_mode - 1;
	if (ctxx.pattern == NULL)
		return (expansion_destroy(tmp), -1);
	expand_promote(exp, tmp);
	ctxx.i = 0;
	ctxx.comp_pattern = pattern_compile(ctxx.pattern, ctxx.info);
	free(ctxx.pattern);
	free(ctxx.info);
	expand_pattern_do(&ctxx, exp, ctx);
	return (pattern_destroy(ctxx.comp_pattern), 0);
}

/* This function literally only calls expansion_destroy.
	this is meant for norminette*/
static void
	ed(t_expand *exp)
{
	expansion_destroy(exp);
}

int
	expand_subst(t_expand *exp, t_param_ctx *ctx, int mode)
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
		return (ed(&tmp), expansion_add_part(exp, sh_strlst_empty(), 0), 0);
	if ((ctx->token->str[ctx->i] == '-' && !empty)
		|| (ctx->token->str[ctx->i] == '+' && empty))
		return (expand_promote(exp, &tmp));
	expansion_destroy(&tmp);
	if (ctx->token->str[ctx->i] == '+' || ctx->token->str[ctx->i] == '-')
		return (expand_right(exp, ctx, ctx->i + 1, mode));
	if (ctx->token->str[ctx->i] == '?')
		return (expand_error(ctx, ctx->i + 1));
	if (ctx->token->str[ctx->i] == '=')
		return (expand_assign(exp, ctx, ctx->i + 1, mode));
	return (-1);
}

int
	expand_param(t_expand *exp, t_token *token, int mode)
{
	t_expand	tmp;
	t_param_ctx	ctx;
	int			result;

	ctx.token = token;
	if (token->str[0] == '#' && (token->str[1] != '#'
			|| token->str[2] == '\0') && token->str[1] != '\0')
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
	result = expand_subst(exp, &ctx, mode);
	free(ctx.key);
	return (result);
}
