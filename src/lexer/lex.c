/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lex.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 16:03:48 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/22 16:27:58 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static int
	is_blank(int c)
{
	return (c == ' ' || c == '\t');
}

static int
	check_op(t_lexer *lex)
{
	if (lex->depth == 0 && lex->src->cur == ')' && lex->id == lx_command)
	{
		lex->tok->id = op_rparen;
		return (0);
	}
	lex->tok->id = lex_op(lex);
	if (lex->tok->id == op_lparen)
		lex->depth += 1;
	if (lex->tok->id == op_rparen)
		lex->depth -= 1;
	return (1);
}

void
	lex_skip(t_lexer *lex)
{
	if (lex->src->cur == -1)
		lex_advance(lex);
	while (is_blank(lex->src->cur) && !lex_quoted(lex))
		lex_advance(lex);
	if (lex->src->cur == '#' && !lex_quoted(lex))
		while (lex->src->cur != '\n' && lex->src->cur != -1)
			lex_advance(lex);
}

int
	lex_lex(t_lexer *lex, t_token *tok)
{
	int	status;

	lex_skip(lex);
	lex->tok = tok;
	token_init(tok);
	if (lex->src->cur == '\n')
	{
		tok->id = tk_newline;
		lex_nom_and_skip(lex);
		return (1);
	}
	if (lex->src->cur == -1)
	{
		token_destroy(tok);
		return (0);
	}
	status = check_op(lex);
	if (tok->id != tk_null)
	{
		return (status);
	}
	token_add_part(lex->tok, lx_normal);
	tok->id = tk_word;
	lex_main(lex);
	return (1);
}
