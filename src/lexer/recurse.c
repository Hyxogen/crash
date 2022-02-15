#include "lexer.h"

static int
	lexer_special_dollar(t_lexer *lex)
{
	if (lex->next == '(')
	{
		lex_read(lex, 1);
		lex_read(lex, 1);
		if (lex->next == '(')
		{
			lex_read(lex, 1);
			lexer_expand_dollar(lex, xp_arithmetic);
			return (1);
		}
		lexer_expand_dollar(lex, xp_command);
		return (1);
	}
	if (lex->next == '{')
	{
		lex_read(lex, 1);
		lex_read(lex, 1);
		lexer_expand_dollar(lex, xp_parameter);
		return (1);
	}
	return (0);
}

static int
	lexer_special(t_lexer *lex)
{
	if (lex->cur == '\'')
	{
		lexer_read(lex, 1);
		lex->quote = 1 - lex->quote;
		return (1);
	}
	if (lex->cur == '"')
	{
		lexer_read(lex, 1);
		lex->quote = 2 - lex->quote;
		return (1);
	}
	if (lex->cur == '$')
	{
		return lexer_special_dollar(lex);
	}
	if (lex->cur == '`')
	{
		lexer_read(lex, 1);
		lexer_expand_backtick(lex);
		return (1);
	}
	return (0);
}

static int
	lexer_word_end(t_lexer *lex)
{
	if (lex->tok->xps[lex->tok->count - 1].id == xp_command)
		return (lex->cur == ')' && !lexer_quoted(lex));
	if (lex->tok->xps[lex->tok->count - 1].id == xp_parameter)
		return (lex->cur == '}' && !lexer_quoted(lex));
	if (lex->tok->xps[lex->tok->count - 1].id == xp_arithmetic)
		return (lex->cur == ')' && lex->next == ')' && !lexer_quoted(lex));
	return (ft_strchr("\n\t &();<>|", lex->cur) != NULL && !lexer_quoted(lex));
}

void
	lexer_recurse(t_lexer *lex)
{
	while (!lexer_word_end(lex))
		if (lexer_quoted(lex) || !lexer_special(lex))
			lexer_read(lex, 0);
}