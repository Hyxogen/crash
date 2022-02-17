#include "lexer.h"
#include "minishell.h"

static t_expansion
	*lexer_expand(t_token *tok)
{
	size_t		old_size;
	size_t		new_size;
	t_expansion	*xp;

	old_size = (tok->count + 0) * sizeof(*tok->xps);
	new_size = (tok->count + 1) * sizeof(*tok->xps);
	tok->xps = sh_safe_realloc(tok->xps, old_size, new_size);
	xp = &tok->xps[tok->count];
	xp->string = NULL;
	xp->length = 0;
	tok->count += 1;
	return (xp);
}

static void
	lexer_state_push(t_lexer *lex, t_lexer_state *state, t_expansion_id id)
{
	t_expansion	*xp;

	state->quote = lex->quote;
	state->was_word = 0;
	state->end = lex->end;
	state->xp_id = lex->xp_id;
	lex->quote = 0;
	lex->end = NULL;
	lex->xp_id = id;
	if (lex->tok->xps[lex->tok->count - 1].id == xp_word)
	{
		xp = lexer_expand(lex->tok);
		xp->id = id;
		xp->quoted = state->quote > 0;
		state->was_word = 1;
	}
}

static void
	lexer_state_pop(t_lexer *lex, t_lexer_state *state)
{
	t_token		*tok;
	t_expansion	*xp;

	tok = lex->tok;
	lex->quote = state->quote;
	lex->end = state->end;
	lex->xp_id = state->xp_id;
	if (state->was_word)
	{
		xp = lexer_expand(lex->tok);
		xp->id = xp_word;
		xp->quoted = 0;
	}
}

int
	lexer_expand_dollar(t_lexer *lex, t_expansion_id id)
{
	t_lexer_state	state;
	int				skip;
	int				status;

	lexer_state_push(lex, &state, id);
	status = lexer_recurse(lex);
	lexer_state_pop(lex, &state);
	if (id == xp_arithmetic)
		skip = 2;
	else
		skip = 1;
	if (status < 0)
		return (status);
	while (skip > 0)
	{
		lexer_read(lex, 1);
		skip -= 1;
	}
	return (0);
}

int
	lexer_expand_backtick(t_lexer *lex)
{
	t_lexer_state	state;

	lexer_state_push(lex, &state, xp_command);
	lex->btick = 1;
	while ((lex->cur != '`' || lexer_quoted(lex)) && lex->cur != -1)
		lexer_read(lex, 0);
	lex->btick = 0;
	lexer_state_pop(lex, &state);
	if (lex->cur == -1)
		return (-1);
	lexer_read(lex, 1);
	return (0);
}
