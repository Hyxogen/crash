/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   convert.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/16 14:13:22 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/16 14:13:59 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <libft.h>

static const char	*g_pr_keywords[] = {
	"if",
	"then",
	"else",
	"elif",
	"fi",
	"do",
	"done",
	"case",
	"esac",
	"while",
	"until",
	"for",
	"{",
	"}",
	"!",
	"in"
};

/* TODO */
static int
	pr_is_name(t_token *token)
{
	(void) token;
	return (1);
}

int
	pr_convert_io_number(t_parser *pr, t_token *token)
{
	size_t	i;

	if (pr->lexer->ch1 != '<' && pr->lexer->ch1 != '>')
		return (0);
	i = 0;
	while (i < token->length)
	{
		if (!ft_isdigit(token->string[i]))
			return (0);
		i += 1;
	}
	token->id = tk_ionumber;
	return (1);
}

int
	pr_convert_keyword(t_parser *pr, t_token *token)
{
	size_t	i;
	size_t	len;

	(void) pr;
	i = 0;
	while (i < 16)
	{
		len = ft_strlen(g_pr_keywords[i]);
		if (len == token->length && ft_memcmp(g_pr_keywords[i], token->string, len) == 0)
		{
			token->id = i + kw_if;
			return (1);
		}
	}
	return (0);
}

int
	pr_convert_7b(t_parser *pr, t_token *token)
{
	(void) pr;
	if (token->length >= 1 && *token->string == '=')
		return (0);
	if (ft_memchr(token->string, '=', token->length) != NULL)
	{
		token->id = tk_assword;
		return (1);
	}
	return (0);
}

int
	pr_convert_name(t_parser *pr, t_token *token)
{
	(void) pr;
	if (pr_is_name(token))
	{
		token->id = tk_name;
		return (1);
	}
	return (0);
}
