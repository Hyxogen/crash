#include "lexer.h"

int
	lexer_new(t_lexer *lex, t_input *in)
{
	lex->in = in;
	lex->tok = NULL;
	lex->xp = NULL;
	lex->cur = -1;
	lex->next = -1;
	lex->quote = 0;
	lex->arith = 0;
	lex->bslash = 0;
	lex->btick = 0;
	return (0);
}

void
	lexer_delete(t_lexer *lex)
{
	(void) lex;
}

int
	token_new(t_token *tok, t_token_id id)
{
	tok->id = id;
	tok->string = NULL;
	tok->length = 0;
	tok->xps = NULL;
	tok->count = 0;
	return (0);
}

void
	token_delete(t_token *tok)
{
	size_t  i;

	free(tok->string);
	i = 0;
	while (i < tok->count)
	{
		free(tok->xps[i].string);
		i += 1;
	}
}