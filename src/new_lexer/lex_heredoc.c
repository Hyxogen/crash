/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lex_heredoc.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/22 10:52:43 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/22 11:07:51 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "new_lexer.h"

void
	lex_heredoc(t_lexer *lex, t_token *tok, const char *end, int trim)
{
	t_lexer	new_lexer;

	lex_init(&new_lexer);
	new_lexer.prev = lex;
	new_lexer.id = lx_normal;
	new_lexer.src = lex->src;
	new_lexer.end = end;
	new_lexer.trim = trim;
	new_lexer.tok = tok;
	lex_main(&new_lexer);
	/* TODO: destroy resources */
}
