#include "lexer.h"
#include "minishell.h"

static void
	lexer_state_push(t_lexer *lex, t_lexer_state *state, t_expansion_id id)
{
	t_token	*tok;

	tok = lex->tok;
	state->quote = lex->quote;
	state->arith = lex->arith;
	state->xp = lex->xp;
	lex->quote = 0;
	lex->arith = id == xp_arithmetic;
	if (lex->xp == NULL)
	{
		tok->xps = sh_safe_realloc(tok->xps, tok->count, tok->count + 1);
		tok->xps[tok->count].id = id;
		tok->xps[tok->count].string = NULL;
		tok->xps[tok->count].length = 0;
		tok->xps[tok->count].quoted = 0;
	}
}

static void
	lexer_state_pop(t_lexer *lex, t_lexer_state *state)
{
	lex->quote = state->quote;
	lex->arith = state->arith;
	lex->xp = state->xp;
}

void
	lexer_expand_dollar(t_lexer *lex, t_expansion_id id)
{
	t_lexer_state	state;

	lexer_state_push(lex, &state, id);
	lexer_recurse(lex);
	lexer_state_pop(lex, &state);
}

void
	lexer_expand_backtick(t_lexer *lex)
{
	t_lexer_state	state;

	lexer_state_push(lex, &state, xp_command);
	lex->btick = 1;
	while ((lex->cur != '`' || lexer_quoted(lex)) && lex->cur != -1)
		lexer_read(lex, 0);
	lexer_read(lex, 1);
	lex->btick = 0;
	lexer_state_pop(lex, &state)
}