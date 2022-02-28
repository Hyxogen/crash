/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   advance.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 16:05:07 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/28 16:05:22 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

#include "memory.h"

void
	lex_append(t_lexer *lex, char **string, size_t *length, int esc)
{
	if (lex->src->cur == -1)
		return ;
	if (lex->src->bslash && esc)
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

	super_lexer = lex;
	if (super_lexer != NULL)
	{
		tmp = super_lexer->src->cur;
		super_lexer->src->cur = ch;
		lex = lex->prev;
		while (lex != NULL)
		{
			lex_append(super_lexer, &lex->tok->str, &lex->tok->len, 1);
			lex = lex->prev;
		}
		super_lexer->src->cur = tmp;
	}
}

void
	lex_nom_and_skip(t_lexer *lex)
{
	lex_append(lex, &lex->tok->str, &lex->tok->len, 1);
	lex_nom(lex, lex->src->cur);
	lex->src->cur = -1;
}

void
	lex_advance(t_lexer *lex)
{
	lex_nom(lex, lex->src->cur);
	while (1)
	{
		lex->src->bslash = 0;
		src_advance(lex->src);
		if (lex->src->cur == '\\' && !lex_quoted(lex) && lex_bquoted(lex))
		{
			lex->src->bslash = 1;
			lex->src->cur = lex->src->nex;
			lex->src->nex = -1;
		}
		if (lex->src->cur != '\n' || !lex->src->bslash)
			return ;
	}
}

void
	lex_update(t_lexer *lex, int sep)
{
	t_tpart	*part;

	lex_append(lex, &lex->tok->str, &lex->tok->len, 1);
	if (lex->tok->count > 0)
	{
		part = &lex->tok->parts[lex->tok->count - 1];
		if ((part->id == lx_backtick || part->id == lx_normal) && !sep)
			lex_append(lex, (char **) &part->data, &part->len, 0);
	}
	lex_advance(lex);
}
