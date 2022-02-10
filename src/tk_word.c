/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tk_word.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/07 11:15:38 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/10 14:40:32 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <libft.h>

static int	tk_expand(t_tokenizer *tk, int ch);

static int
	tk_doquote(t_tokenizer *tk)
{
	if (!tk_quoted(tk))
	{
		if (tk->ch == '`')
			return (tk_expand(tk, '`'));
		else if (tk->ch == '$')
		{
			tk_readchar(tk);
			if (tk->ch == '(')
				return (tk_expand(tk, ')'));
			else if (tk->ch == '{')
				return (tk_expand(tk, '}'));
			return (0);
		}
		else if (tk->ch == '\'')
			tk->quote = 1 - tk->quote;
		else if (tk->ch == '\"')
			tk->quote = 2 - tk->quote;
	}
	tk_readchar(tk);
	return (0);
}

static int
	tk_expand(t_tokenizer *tk, int ch)
{
	int	quote;
	int	tmp;

	quote = tk->quote;
	tk->quote = 0;
	tk_readchar(tk);
	while (tk->ch != ch || tk_quoted(tk))
	{
		if (tk->ch == -1)
			return (token_id_null);
		if (tk->ch == '(' && !tk_quoted(tk))
			tmp = tk_expand(tk, ')');
		else if (tk->ch == '{' && !tk_quoted(tk))
			tmp = tk_expand(tk, '}');
		else
			tmp = tk_doquote(tk);
		if (tmp < 0)
			return (tmp);
	}
	tk_readchar(tk);
	tk->quote = quote;
	return (0);
}

t_token_id
	tk_word(t_tokenizer *tk)
{
	int	tmp;

	while (ft_strchr("\n\t &();<>|", tk->ch) == NULL
		|| tk_quoted(tk) || tk->ch == '\0')
	{
		if (tk->ch == -1)
			return (token_id_null);
		tmp = tk_doquote(tk);
		if (tmp < 0)
			return (tmp);
	}
	return (word);
}
