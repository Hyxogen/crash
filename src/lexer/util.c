#include "lexer.h"

void
	lexer_skip(t_lexer *lex)
{
	if (lex->cur == -1)
		lexer_read(lex, 0);
	while ((lex->cur == ' ' || lex->cur == '\t') && !lexer_quoted(lex))
		lexer_read(lex, 0);
	if (lex->cur == '#' && !lexer_quoted(lex))
		while (lex->cur != '\n' && lex->cur != -1)
			lexer_read_raw(lex->in);
}