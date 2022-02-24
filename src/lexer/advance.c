#include "lexer.h"

#include "memory.h"

static void
	append(t_lexer *lex, char **string, size_t *length, int esc)
{
	if (lex->src->cur == -1)
		return ;
	if (lex->bslash && esc)
	{
		*string = sh_safe_realloc(*string, *length, *length + 2);
		(*string)[*length] = '\\';
		*length += 1;
		(*string)[*length] = '\0';
	}
	*string = sh_safe_realloc(*string, *length, *length + 2);
	(*string)[*length] = lex->src->cur;
	*length += 1;
	(*string)[*length] = '\0';
}

void
	lex_nom(t_lexer *lex, int ch)
{
	int		tmp;
	t_lexer	*super_lexer;

	printf("nom: %d %c\n", ch, ch);
	super_lexer = lex;
	if (super_lexer != NULL)
	{
		tmp = super_lexer->src->cur;
		super_lexer->src->cur = ch;
		lex = lex->prev;
		while (lex != NULL)
		{
			append(super_lexer, &lex->tok->str, &lex->tok->len, 1);
			lex = lex->prev;
		}
		super_lexer->src->cur = tmp;
	}
}

void
	lex_advance(t_lexer *lex)
{
	lex_nom(lex, lex->src->cur);
	while (1)
	{
		lex->bslash = 0;
		src_advance(lex->src);
		if (lex->src->cur == '\\' && !lex_quoted(lex) && lex_bquoted(lex))
		{
			lex->bslash = 1;
			lex->src->cur = lex->src->nex;
			lex->src->nex = -1;
		}
		if (lex->src->cur != '\n' || !lex->bslash)
			return ;
	}
}

void
	lex_update(t_lexer *lex, int sep)
{
	t_tpart	*part;

	append(lex, &lex->tok->str, &lex->tok->len, 1);
	if (lex->tok->count > 0)
	{
		part = &lex->tok->parts[lex->tok->count - 1];
		if ((part->id == lx_backtick || part->id == lx_normal) && !sep)
			append(lex, (char**) &part->data, &part->len, 0);
	}
	lex_advance(lex);
}
