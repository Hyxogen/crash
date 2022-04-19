#include "commander.h"
#include "minishell.h"
#include "memory.h"
#include "libft.h"
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

/* this function is waaaaaaaaAAAAAAYYYY too long */
int
	expand_special(t_expand *exp, char *key)
{
	char	*str;
	long	idx;
	long	i;

	// TODO: special parameters
	i = 0;
	while (sh()->args[i] != NULL)
		i += 1;
	if (key[0] == '*')
	{
		str = sh_getenv("IFS", NULL);
		if (str == NULL)
			str = sh_strlst_join(sh()->args + 1, ' ');
		else
			str = sh_strlst_join(sh()->args + 1, str[0]);
		expansion_add_part(exp, sh_strlst_new(str), 0);
		return (0);
	}
	if (key[0] == '@')
	{
		expansion_add_part(exp, sh_strlst_dup(sh()->args + 1), 0);
		exp->parts[exp->count - 1].array = 1;
		return (0);
	}
	if (key[0] == '#')
	{
		expansion_add_part(exp, sh_strlst_new(ft_itoa(i - 1)), 0);
		return (0);
	}
	if (key[0] == '?')
	{
		expansion_add_part(exp, sh_strlst_new(ft_itoa(sh()->return_code)), 0);
		return (0);
	}
	if (key[0] == '-')
	{
		// TODO: option flags
		expansion_add_part(exp, sh_strlst_new(ft_strdup("")), 0);
		return (0);
	}
	if (key[0] == '$')
	{
		expansion_add_part(exp, sh_strlst_new(ft_itoa(getpid())), 0);
		return (0);
	}
	if (key[0] == '!')
	{
		// TODO: background commands
		expansion_add_part(exp, sh_strlst_new(ft_itoa(0)), 0);
		return (0);
	}
	if (ft_isdigit(key[0]))
	{
		idx = ft_atol(key);
		if (idx < 0 || idx >= i)
			expansion_add_part(exp, sh_strlst_new(ft_strdup("")), 0);
		else
			expansion_add_part(exp, sh_strlst_new(ft_strdup(sh()->args[idx])), 0);
		return (0);
	}
	str = sh_getenv(key, NULL);
	if (str == NULL)
		return (-1);
	expansion_add_part(exp, sh_strlst_new(ft_strdup(str)), 0);
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
		if (str != NULL)
			return (-1);
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
	if (str == NULL)
		return (-1);
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
	int		*info;
	char	*pattern;
	int		long_mode;
	size_t	i;
	size_t	j;
	size_t	best;
	char	*str;
	int		ch;
	int		match;

	long_mode = ctx->token->str[ctx->i] == ctx->token->str[ctx->i + 1];
	info = NULL;
	ctx->token->parts[0].data = (char*) ctx->token->parts[0].data + ctx->i + long_mode + 1;
	pattern = cm_expand_str(ctx->token, &info, ' ');
	ctx->token->parts[0].data = (char*) ctx->token->parts[0].data - ctx->i - long_mode - 1;
	if (pattern == NULL)
	{
		expansion_destroy(tmp);
		return (-1);
	}
	expand_promote(exp, tmp);
	i = 0;
	while (exp->parts[exp->count - 1].str[i] != NULL)
	{
		j = 0;
		str = exp->parts[exp->count - 1].str[i];
		if (ctx->token->str[ctx->i] == '%')
		{
			best = ft_strlen(str);
			while (str[j] != '\0')
			{
				match = pattern_match(str + j, pattern, info);
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
				match = pattern_match(str, pattern, info);
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
		return (expansion_destroy(&tmp), expansion_add_part(exp, sh_strlst_empty(), 0), 0);
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

// TODO: error for invalid pattern

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
