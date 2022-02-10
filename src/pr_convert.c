#include "minishell.h"

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

static int
	pr_is_name(t_token *token)
{
	return (1);
}

int
	pr_convert_io_number(t_parser *pr, t_token *token)
{
	size_t	i;

	if (pr->tokenizer->ch != '<' && pr->tokenizer->ch != '>')
		return (0);
	i = 0;
	while (i < token->len)
	{
		if (!ft_isdigit(token->str[i]))
			return (0);
		i += 1;
	}
	token->id = io_number;
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
		if (len == token->len && ft_memcmp(g_pr_keywords[i], token->str) == 0)
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
	if (token->len >= 1 && *token->str == '=')
		return (0);
	if (ft_memchr(token->str, '=', token->len) != NULL)
	{
		token->id = assignment_word;
		return (1);
	}
	return (0);
}

int
	pr_convert_name(t_parser *pr, t_token *token)
{
	if (pr_is_name(token))
	{
		token->id = name;
		return (1);
	}
	return (0);
}
