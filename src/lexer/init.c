#include "minishell.h"
#include <stdlib.h>

int
	lexer_new(t_lexer *lex, t_input *in)
{
	lex->in = in;
	lex->tok = NULL;
	lex->cur = -1;
	lex->next = -1;
	lex->quote = 0;
	lex->bslash = 0;
	lex->btick = 0;
	lex->end = NULL;
	lex->xp_id = xp_word;
	return (0);
}

void
	lexer_delete(t_lexer *lex)
{
	(void) lex;
}

int
	token_new(t_token *tok)
{
	tok->id = tk_null;
	tok->string = NULL;
	tok->length = 0;
	tok->xps = NULL;
	tok->count = 0;
	return (0);
}

int
	token_start(t_token *tok)
{
	tok->xps = sh_safe_malloc(sizeof(*tok->xps));
	tok->xps[0].id = xp_word;
	tok->xps[0].string = NULL;
	tok->xps[0].length = 0;
	tok->xps[0].quoted = 0;
	tok->count += 1;
	return (0);
}

void
	token_delete(t_token *tok)
{
	size_t	i;

	free(tok->string);
	i = 0;
	while (i < tok->count)
	{
		free(tok->xps[i].string);
		i += 1;
	}
}
