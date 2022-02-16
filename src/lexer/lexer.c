#include "lexer.h"

static void
	lexer_skip(t_lexer *lex)
{
	if (lex->cur == -1)
		lexer_read(lex, 0);
	while ((lex->cur == ' ' || lex->cur == '\t') && !lexer_quoted(lex))
		lexer_read(lex, 0);
	if (lex->cur == '#' && !lexer_quoted(lex))
		while (lex->cur != '\n' && lex->cur != -1)
			lexer_read_raw(lex);
}

static int
	lexer_word(t_lexer *lex)
{
	int	status;

	token_start(lex->tok);
	lex->tok->id = tk_word;
	status = lexer_recurse(lex);
	if (status < 0)
	{
		token_delete(lex->tok);
		return (status);
	}
	return (1);
}

int
	lexer_lex(t_lexer *lex, t_token *tok)
{
	lex->tok = NULL;
	lex->end = NULL;
	lexer_skip(lex);
	lex->tok = tok;
	token_new(tok);
	if (lex->cur == '\n')
	{
		tok->id = tk_newline;
		lex->cur = lex->next;
		lex->next = -1;
		return (1);
	}
	if (lex->cur == -1)
	{
		token_delete(tok);
		return (0);
	}
	tok->id = lexer_op(lex);
	if (tok->id == tk_null)
		return (lexer_word(lex));
	return (1);
}

int
	lexer_heredoc(t_lexer *lex, t_token *tok, const char *end)
{
	int	status;

	lex->tok = tok;
	lex->end = end;
	token_new(tok);
	if (lex->cur == -1)
		lexer_read(lex, 0);
	status = lexer_word(lex);
	if (status < 0)
		return (status);
	while (lex->cur != '\n' && lex->cur != -1)
		lexer_read(lex, 1);
	lex->cur = lex->next;
	lex->next = -1;
	return (1);
}
