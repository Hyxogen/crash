/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/22 13:52:28 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/25 10:32:05 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void
	src_init(t_source *src, t_input *in)
{
	src->str = NULL;
	src->off = 0;
	src->len = 0;
	src->bslash = 0;
	src->cur = -1;
	src->nex = -1;
	src->esc = 0;
	src->lst = NULL;
	src->in = in;
}

void
	lex_init(t_lexer *lex)
{
	lex->prev = NULL;
	lex->id = lx_normal;
	lex->src = NULL;
	lex->tok = NULL;
	lex->btick = 0;
	lex->quote = 0;
	lex->depth = 0;
	lex->end = NULL;
	lex->here_flags = 0;
}

void
	token_init(t_token *tok)
{
	tok->id = 0;
	tok->parts = NULL;
	tok->count = 0;
	tok->str = NULL;
	tok->len = 0;
}
