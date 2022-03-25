/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   init.c                                           -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:22:45 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:22:45 by csteenvo            :      ..            */
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
	lex->error = 0;
	lex->end = NULL;
	lex->here_flags = 0;
}

void
	token_init(t_token *tok)
{
	tok->id = tk_invalid;
	tok->parts = NULL;
	tok->count = 0;
	tok->str = NULL;
	tok->len = 0;
}
