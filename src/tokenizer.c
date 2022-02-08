/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/07 11:15:38 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/08 10:21:06 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <libft.h>

static int
	tk_isword(int ch)
{
	return (0);
}

static void
	tk_subst(t_tokenizer *tk, int ch)
{
	sh_assert(0 && "tk_subst not implemented");
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
			tk->ch = sh_readchar(&tk->in);
	while (1)
	{
		if (tk_isquote(tk, tk->ch) || tk_isword(tk->ch))
			tk_advance(tk);
		else if (tk->ch == '\\' || tk->ch == '\'' || tk->ch == '\"')
			tk_quote(tk, tk->ch);
		else if (tk->ch == '$' || tk->ch == '`')
			tk_subst(tk, tk->ch);
		else if (isspace(tk->ch))
		{
			if (tk->ch != '\n')
				tk->ch = tk_readchar(tk);
			return (1);
		}
	}
}
