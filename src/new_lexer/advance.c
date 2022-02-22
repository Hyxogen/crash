#include "new_lexer.h"

static void
	update(t_lexer *lex, int top)
{

}

void
	lex_advance(t_lexer *lex)
{
	src_advance(lex->src);
	lex = lex->next;
	while (lex != NULL)
	{
		update(lex, 0);
		lex = lex->next;
	}
}

void
	lex_update(t_lexer *lex)
{
	lex_advance(lex);
	update(lex, 1);
}