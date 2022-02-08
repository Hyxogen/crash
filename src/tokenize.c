/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/07 11:15:38 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/08 16:23:14 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <libft.h>

static void
	tk_skip(t_tokenizer *tk)
{
	if (tk->ch == -1)
		tk_readchar(tk);
	while ((tk->ch == ' ' || tk->ch == '\t') && !tk_quoted(tk))
		tk_readchar(tk);
	if (tk->ch == '#' && !tk_quoted(tk))
		while (tk->ch != '\n' && tk->ch != -1)
			tk->ch = sh_readchar(tk->in);
}

static void
	tk_expand(t_tokenizer *tk)
{
	(void) tk;
	sh_assert(0 && "tk_expand not implemented");
}

static void
	tk_word(t_tokenizer *tk)
{
	while (ft_strchr("\n\t &();<>|", tk->ch) == NULL
		|| tk_quoted(tk) || tk->ch == '\0')
	{
		if (!tk_quoted(tk) && (tk->ch == '$' || tk->ch == '`'))
			tk_expand(tk);
		tk_append(tk);
		if (!tk_quoted(tk) && tk->ch == '\'')
			tk->quote = 1 - tk->quote;
		else if (!tk_quoted(tk) && tk->ch == '\"')
			tk->quote = 2 - tk->quote;
		tk_readchar(tk);
	}
}

int
	tk_tokenize(t_tokenizer *tk)
{
	tk->tok->str = NULL;
	tk->tok->len = 0;
	tk_skip(tk);
	if (tk->ch == -1)
		return (0);
	tk->tok->id = newline;
	if (tk->ch == '\n')
	{
		tk_append(tk);
		tk->ch = -1;
		return (1);
	}
	tk->tok->id = tk_op(tk);
	if (tk->tok->id != token_id_null)
		return (1);
	tk->tok->id = word;
	tk_word(tk);
	return (1);
}
