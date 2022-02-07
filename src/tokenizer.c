/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/07 11:15:38 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/07 16:34:49 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <libft.h>

static int
	tk_isquote(t_tokenizer *tk)
{
	return (0);
}

static int
	tk_isoper(t_tokenizer *tk)
{
	return (0);
}

static void
	tk_quote(t_tokenizer *tk)
{
}

static void
	tk_subst(t_tokenizer *tk)
{
}

int
	tk_tokenize(t_tokenizer *tk)
{
	if (tk->ch == -1)
		return (0);
	tk->tok->id = tk_op(tk);
	if (tk->tok->id != token_id_null)
		return (1);
	if (tk->ch == '#')
		while (tk->ch != '\n')
			tk->ch = tk_readchar_raw(tk);
	while (1)
	{
		if (!tk_isquote(tk, tk->ch))
		{
			if (tk->ch == '\\' || tk->ch == '\'' || tk->ch == '\"')
			{
				tk_quote(tk, tk->ch);
				continue ;
			}
			if (tk->ch == '$' || tk->ch == '`')
			{
				tk_subst(tk, tk->ch);
				continue ;
			}
			if (ft_isspace(tk->ch) && tk->ch != '\n')
				tk->ch = tk_readchar(tk);
			if (ft_isspace(tk->ch) || tk_isoper(tk->ch))
				return (1);
		}
		tk_advance(tk);
	}
}
