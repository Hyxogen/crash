/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer_util.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/07 15:28:40 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/08 12:17:13 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <libft.h>

void
	tk_append(t_token *t, int ch)
{
	size_t	len;

	len = ft_strlen(t->str);
	t->str = sh_safe_reallloc(t->str, len, len + 1);
	t->str[len] = ch;
	t->str[len + 1] = '\0';
}

int
	tk_isquote(t_tokenizer *tk, int ch)
{
	return (0);
}

void
	tk_quote(t_tokenizer *tk, int ch)
{
	sh_assert(0 && "tk_quote not implemented");
}

void
	tk_readchar(t_tokenizer *tk)
{
	if (tk->bslash)
	{
		tk->
	}
}

void
	tk_advance(t_tokenizer *tk)
{
	tk_append(tk->tok, tk->ch);
	tk->ch = tk_readchar(tk);
}

int
	tk_assume(t_tokenizer *tk, int ch)
{
	while (tk->ch == '\\' && tk->next_ch == '\n')
	{
		tk->ch = sh_readchar(&tk->in);
		tk->next_ch = sh_readchar(&tk->in);
	}
	if (tk->ch == ch)
	{
		tk_append(tk->tok, ch);
		tk->ch = tk_readchar_raw(tk);
		return (1);
	}
	return (0);
}
