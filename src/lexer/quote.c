#include "lexer.h"

int
	lexer_quoted(t_lexer *lex)
{
	if (lex->bslash)
		return (1);
	if (lex->quote == 1)
	{
		if (lex->cur == '\'')
			return (0);
		return (1);
	}
	if (lex->quote == 2 || lex->end != NULL)
	{
		if (lex->cur == '"' && lex->end == NULL)
			return (0);
		if (lex->cur == '\\')
			return (0);
		if (lex->cur == '$')
			return (0);
		if (lex->cur == '`')
			return (0);
		return (1);
	}
	return (0);
}

int
	lexer_bquoted(t_lexer *lex)
{
	if (lex->btick)
	{
		if (lex->next == '$')
			return (1);
		if (lex->next == '`')
			return (1);
		if (lex->next == '\\')
			return (1);
		return (0);
	}
	if (lex->quote == 2 || lex->end != NULL)
	{
		if (lex->next == '$')
			return (1);
		if (lex->next == '\'')
			return (1);
		if (lex->next == '"')
			return (1);
		if (lex->next == '\\')
			return (1);
		if (lex->next == '\n')
			return (1);
		return (0);
	}
	return (1);
}
