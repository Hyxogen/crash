#include "lexer.h"
#include "minishell.h"

static void
	lexer_append(t_lexer *lex, char **string, size_t *length, int esc)
{
	if (lex->bslash && esc)
	{
		*string = sh_safe_realloc(*string, *length, *length + 2);
		(*string)[*length] = '\\';
		*length += 1;
		(*string)[*length] = '\0';
	}
	*string = sh_safe_realloc(*string, *length, *length + 2);
	(*string)[*length] = lex->cur;
	*length += 1;
	(*string)[*length] = '\0';
}

static void
	lexer_update(t_lexer *lex, int xp_sep)
{
	t_expansion	*xp;

	if (lex->cur != -1 && lex->tok != NULL)
	{
		lexer_append(lex, &lex->tok->string, &lex->tok->length, 1);
		if (lex->tok->count > 0)
		{
			xp = &lex->tok->xps[lex->tok->count - 1];
			if (xp->id != xp_word)
				lexer_append(lex, &xp->string, &xp->length, !lex->btick);
			else if (!xp_sep)
				lexer_append(lex, &xp->string, &xp->length, 0);
		}
	}
}

void
	lexer_read(t_lexer *lex, int xp_sep)
{
	lexer_update(lex, xp_sep);
	while (1)
	{
		lex->bslash = 0;
		lex->cur = lex->next;
		lex->next = -1;
		if (lex->cur == -1)
			lex->cur = input_readchar(lex->in);
		if (lex->cur != -1 && lex->cur != '\n')
			lex->next = input_readchar(lex->in);
		if (lex->cur == '\\' && !lexer_quoted(lex) && lexer_bquoted(lex))
		{
			lex->bslash = 1;
			lex->cur = lex->next;
			lex->next = -1;
		}
		if (lex->cur != '\n' || !lex->bslash)
			return ;
	}
}

void
	lexer_read_raw(t_lexer *lex)
{
	lexer_update(lex, 0);
	lex->bslash = 0;
	lex->cur = lex->next;
	lex->next = -1;
	if (lex->cur < 0)
		lex->cur = input_readchar(lex->in);
	lex->next = input_readchar(lex->in);
}
