#include "lexer.h"
#include <libft.h>

static int
	lexer_special_dollar(t_lexer *lex)
{
	if (lex->next == '(')
	{
		lexer_read(lex, 1);
		lexer_read(lex, 1);
		if (lex->cur == '(')
		{
			lexer_read(lex, 1);
			lexer_expand_dollar(lex, xp_arithmetic);
			return (1);
		}
		lexer_expand_dollar(lex, xp_command);
		return (1);
	}
	if (lex->next == '{')
	{
		lexer_read(lex, 1);
		lexer_read(lex, 1);
		lexer_expand_dollar(lex, xp_parameter);
		return (1);
	}
	return (0);
}

static int
	lexer_special(t_lexer *lex)
{
	if (lex->cur == '\'' && !lex->btick)
	{
		lex->quote = 1 - lex->quote;
		lexer_read(lex, 1);
		return (1);
	}
	if (lex->cur == '"' && !lex->btick)
	{
		lex->quote = 2 - lex->quote;
		lexer_read(lex, 1);
		return (1);
	}
	if (lex->cur == '$' && !lex->btick)
	{
		return lexer_special_dollar(lex);
	}
	if (lex->cur == '`' && !lex->btick)
	{
		lexer_read(lex, 1);
		lexer_expand_backtick(lex);
		return (1);
	}
	return (0);
}

static int
	lexer_bracket(t_lexer *lex)
{
	t_expansion_id	id;

	id = lex->tok->xps[lex->tok->count - 1].id;
	if (id == xp_command || id == xp_arithmetic)
	{
		if (lex->cur == '(')
		{
			lexer_read(lex, 1);
			lexer_expand_dollar(lex, xp_command);
			return (1);
		}
	}
	else if (id == xp_parameter)
	{
		if (lex->cur == '{')
		{
			lexer_read(lex, 1);
			lexer_expand_dollar(lex, xp_parameter);
			return (1);
		}
	}
	return (0);
}

static int
	lexer_word_end(t_lexer *lex)
{
	static const char	*space = "\n\t\r\v\f &();<>|";
	int					is_end;

	if (lex->tok->xps[lex->tok->count - 1].id == xp_command)
		return (lex->cur == ')' && !lexer_quoted(lex));
	if (lex->tok->xps[lex->tok->count - 1].id == xp_parameter)
		return (lex->cur == '}' && !lexer_quoted(lex));
	if (lex->tok->xps[lex->tok->count - 1].id == xp_arithmetic)
		return (lex->cur == ')' && lex->next == ')' && !lexer_quoted(lex));
	if (lex->end != NULL)
		return (lexer_ioend(lex));
	is_end = ft_strchr(space, lex->cur) != NULL && lex->cur != '\0';
	return ((is_end && !lexer_quoted(lex)) || lex->cur == -1);
}

int
	lexer_recurse(t_lexer *lex)
{
	while (!lexer_word_end(lex))
	{
		if (lex->cur == -1)
			return (-1);
		if (lexer_quoted(lex) || (!lexer_special(lex) && !lexer_bracket(lex)))
			lexer_read(lex, 0);
	}
	return (0);
}
