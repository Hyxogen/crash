/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   convert.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/16 14:13:22 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/22 15:02:32 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include <libft.h>

static const char	*g_keywords[] = {
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

static int
	pr_is_name(const char *str, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i += 1;
	}
	return (1);
}

/* rule 1 */
int
	pr_convert_reserved(t_parser *pr, t_token *token)
{
	size_t	i;

	i = 0;
	while (i < sizeof(g_keywords) / sizeof(*g_keywords))
	{
		if (pr_convert_keyword(pr, token, kw_if + i))
			return (1);
		i += 1;
	}
	return (0);
}

/* rule 4, 6 */
int
	pr_convert_keyword(t_parser *pr, t_token *token, t_token_id id)
{
	size_t	len;

	(void) pr;
	if (token == NULL || token->id != tk_word)
		return (0);
	len = ft_strlen(g_keywords[id - kw_if]);
	if (ft_memcmp(g_keywords[id - kw_if], token->str, len + 1) == 0)
	{
		token->id = id;
		return (1);
	}
	return (0);
}

/* rule 5 */
int
	pr_convert_name(t_parser *pr, t_token *token)
{
	(void) pr;
	if (token == NULL || token->id != tk_word)
		return (0);
	if (pr_is_name(token->str, token->len))
	{
		token->id = tk_name;
		return (1);
	}
	return (0);
}

/* rule 7 */
int
	pr_convert_ass(t_parser *pr, t_token *token, int first)
{
	char	*end;

	if (token == NULL || token->id != tk_word)
		return (0);
	end = ft_memchr(token->str, '=', token->len);
	if (end != NULL)
	{
		if (token->str[0] == '=')
			return (0);
		if (!pr_is_name(token->str, end - token->str))
			return (0);
		token->id = tk_assword;
		return (1);
	}
	else if (first)
	{
		pr_convert_reserved(pr, token);
	}
	return (0);
}
