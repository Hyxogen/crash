#include "commander.h"

#include "minishell.h"
#include "memory.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

int
	expand_assign(t_expand *exp, t_param_ctx *ctx, size_t i, int mode)
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
	ctx->token->parts[0].data = (char *) ctx->token->parts[0].data + i;
	str = cm_expand_str(ctx->token, NULL, ' ', mode);
	ctx->token->parts[0].data = (char *) ctx->token->parts[0].data - i;
	if (str == NULL)
		return (-1);
	sh_setenv(ctx->key, str, 0);
	expansion_add_part(exp, sh_strlst_new(str), 0);
	return (0);
}

int
	expand_right(t_expand *exp, t_param_ctx *ctx, size_t i, int mode)
{
	t_expand	tmp;
	int			result;

	ctx->token->parts[0].data = (char *) ctx->token->parts[0].data + i;
	result = cm_expand_list(&tmp, ctx->token, mode);
	ctx->token->parts[0].data = (char *) ctx->token->parts[0].data - i;
	if (result < 0)
		return (-1);
	expansion_copy_parts(exp, &tmp);
	expansion_destroy(&tmp);
	return (0);
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

/* TODO: this function is waaaaAAAYYYY too long */
int
	expand_pattern(t_expand *exp, t_param_ctx *ctx, t_expand *tmp)
{
	int				*info;
	char			*pattern;
	int				long_mode;
	size_t			i;
	size_t			j;
	size_t			best;
	char			*str;
	int				ch;
	int				match;
	t_pattern_node	*comp_pattern;

	long_mode = ctx->token->str[ctx->i] == ctx->token->str[ctx->i + 1];
	info = NULL;
	ctx->token->parts[0].data = (char *) ctx->token->parts[0].data + ctx->i + long_mode + 1;
	pattern = cm_expand_str(ctx->token, &info, ' ', 1);
	ctx->token->parts[0].data = (char *) ctx->token->parts[0].data - ctx->i - long_mode - 1;
	if (pattern == NULL)
	{
		expansion_destroy(tmp);
		return (-1);
	}
	expand_promote(exp, tmp);
	i = 0;
	comp_pattern = pattern_compile(pattern, info);
	free(pattern);
	free(info);
	while (exp->parts[exp->count - 1].str[i] != NULL)
	{
		j = 0;
		str = exp->parts[exp->count - 1].str[i];
		if (ctx->token->str[ctx->i] == '%')
		{
			best = ft_strlen(str);
			while (str[j] != '\0')
			{
				match = pattern_match(str + j, comp_pattern, 0);
				if (match)
				{
					best = j;
					if (long_mode)
						break ;
				}
				j += 1;
			}
			str[best] = '\0';
		}
		else
		{
			best = 0;
			while (str[j] != '\0')
			{
				ch = str[j];
				str[j] = '\0';
				match = pattern_match(str, comp_pattern, 0);
				str[j] = ch;
				if (match)
				{
					best = j;
					if (!long_mode)
						break ;
				}
				j += 1;
			}
			exp->parts[exp->count - 1].str[i] = ft_strdup(str + best);
			free(str);
		}
		i += 1;
	}
	pattern_destroy(comp_pattern);
	return (0);
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
