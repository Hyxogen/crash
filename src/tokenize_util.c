/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize_util.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/07 15:28:40 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/08 16:29:30 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <libft.h>

int
	tk_quoted(t_tokenizer *tk)
{
	if (tk->bslash)
		return (1);
	if (tk->quote == 1 && tk->ch != '\'')
		return (1);
	if (tk->quote != 2)
		return (0);
	if (tk->ch == '\"')
		return (0);
	if (tk->ch == '\\')
		return (0);
	if (tk->ch == '$')
		return (0);
	if (tk->ch == '`')
		return (0);
	return (1);
}

void
	tk_readchar(t_tokenizer *tk)
{
	while (1)
	{
		tk->bslash = 0;
		tk->ch = tk->ch2;
		tk->ch2 = -1;
		if (tk->ch < 0)
			tk->ch = sh_readchar(tk->in);
		if (tk->ch == '\\' && !tk_quoted(tk))
		{
			tk->ch2 = sh_readchar(tk->in);
			if (tk->ch2 == '$' || tk->ch2 == '`' || tk->ch2 == '"'
				|| tk->ch2 == '\\' || tk->ch2 == '\n' || tk->quote != 2)
			{
				tk->ch = tk->ch2;
				tk->ch2 = -1;
				tk->bslash = 1;
			}
		}
		if (tk->ch != '\n' || !tk->bslash)
			return ;
	}
}

void
	tk_append(t_tokenizer *tk)
{
	t_token	*tok;

	tok = tk->tok;
	if (tk->bslash)
	{
		tok->str = sh_safe_realloc(tok->str, tok->len, tok->len + 1);
		tok->str[tok->len] = '\\';
		tok->len += 1;
	}
	tok->str = sh_safe_realloc(tok->str, tok->len, tok->len + 1);
	tok->str[tok->len] = tk->ch;
	tok->len += 1;
}
