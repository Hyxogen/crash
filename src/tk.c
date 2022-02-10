/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tk.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/07 11:15:38 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/10 14:30:00 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <libft.h>
#include <unistd.h>
#include <stdlib.h>

static const char	*g_tk_names[] = {
	"word",
	"assignment_word",
	"name",
	"newline",
	"io_number",
	"op_and_if",
	"op_or_if",
	"op_dsemi",
	"op_dless",
	"op_dgreat",
	"op_lessand",
	"op_greatand",
	"op_lessgreat",
	"op_dlessdash",
	"op_clobber",
	"kw_if",
	"kw_then",
	"kw_else",
	"kw_elif",
	"kw_fi",
	"kw_do",
	"kw_done",
	"kw_case",
	"kw_esac",
	"kw_while",
	"kw_until",
	"kw_for",
	"lbrace",
	"rbrace",
	"bang",
	"kw_in",
	"op_and",
	"op_pipe",
	"op_lparen",
	"op_rparen",
	"op_less",
	"op_great",
	"op_semi"
};

void
	tk_create(t_tokenizer *tk, t_input *in)
{
	tk->in = in;
	tk->str = NULL;
	tk->len = 0;
	tk->ch = -1;
	tk->ch2 = -1;
	tk->bslash = 0;
	tk->quote = 0;
}

void
	tk_destroy(t_tokenizer *tk)
{
	free(tk->str);
}

t_token_id
	tk_run(t_tokenizer *tk)
{
	int	id;

	if (tk->ch == '\n')
	{
		tk_append(tk);
		tk->ch = -1;
		return (newline);
	}
	id = tk_op(tk);
	if (id != token_id_null)
		return (id);
	return (tk_word(tk));
}

int
	tk_tokenize(t_tokenizer *tk, t_token *token)
{
	size_t	start;

	tk_skip(tk);
	if (tk->ch == -1)
		return (0);
	start = tk->len;
	token->id = tk_run(tk);
	if (token->id == token_id_null)
		return (-1);
	token->str = sh_safe_malloc(tk->len - start);
	token->len = tk->len - start;
	ft_memcpy(token->str, tk->str + start, token->len);
	free(tk->str);
	tk->str = NULL;
	tk->len = 0;
	return (1);
}

void
	tk_debug(void)
{
	t_input		in;
	t_tokenizer	tk;
	t_token		token;

	in.line = NULL;
	in.index = 0;
	in.more = 0;
	tk_create(&tk, &in);
	while (1)
	{
		if (tk_tokenize(&tk, &token) != 1)
			break ;
		ft_putstr_fd((char *) g_tk_names[token.id], STDOUT_FILENO);
		ft_putstr_fd((char *) ": ", STDOUT_FILENO);
		write(STDOUT_FILENO, token.str, token.len);
		ft_putchar_fd('\n', STDOUT_FILENO);
		free(token.str);
	}
	free(in.line);
}
