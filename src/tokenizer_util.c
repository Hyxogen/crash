/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer_util.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/07 15:28:40 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/07 16:29:47 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <libft.h>

int
	tk_readchar_raw(t_tokenizer *tk)
{
	int	ch;

	ch = tk->esc_ch;
	tk->esc_ch = sh_readchar(&tk->in);
	return (ch);
}

int
	tk_readchar(t_tokenizer *tk)
{
	int	ch;

	while (1)
	{
		ch = tk_readchar_raw(tk);
		if (ch != '\\' || tk->esc_ch != '\n')
			return (ch);
		ch = tk_readchar_raw(tk);
	}
}

void
	tk_advance(t_tokenizer *tk)
{
	char	buf[2];
	char	*tmp;

	buf[0] = tk->ch;
	buf[1] = '\0';
	if (tk->tok->str != NULL)
		tmp = ft_strjoin(tk->tok->str, buf);
	else
		tmp = ft_strdup(buf);
	free(tk->tok->str);
	tk->tok->str = tmp;
	tk->ch = tk_readchar(tk);
}

int
	tk_assume(t_tokenizer *tk, int ch)
{
	if (tk->ch == ch)
	{
		tk_advance(tk);
		return (1);
	}
	return (0);
}
