/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   special.c                                        -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:22 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:22 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

#include "memory.h"

int
	lex_special_lparen(t_lexer *lex)
{
	t_tpart	*part;

	lex_update(lex, 1);
	if (lex->src->nex == '(')
	{
		lex_update(lex, 1);
		lex_nom_and_skip(lex);
		part = token_add_part(lex, lx_arithmetic, lex->quote);
		lex_arithmetic(lex, part);
		if (lex->src->cur != -1)
			lex_update(lex, 1);
	}
	else
	{
		lex_nom_and_skip(lex);
		part = token_add_part(lex, lx_command, lex->quote);
		lex_command(lex, part);
	}
	if (lex->src->cur != -1)
		lex_update(lex, 1);
	lex->new_part = 1;
	return (1);
}

/* Aka super/extra special parameter */
int
	lex_peculiar_parameter(t_lexer *lex)
{
	t_token	*tmp;

	if (lex->src->nex == '*'
		|| lex->src->nex == '@'
		|| lex->src->nex == '#'
		|| lex->src->nex == '?'
		|| lex->src->nex == '-'
		|| lex->src->nex == '$'
		|| lex->src->nex == '!'
		|| ft_isdigit(lex->src->nex))
	{
		lex_update(lex, 1);
		token_add_part(lex, lx_parameter, lex->quote);
		tmp = lex->tok;
		lex->tok = sh_safe_malloc(sizeof(t_token));
		token_init(lex->tok);
		lex->tok->id = tk_word;
		tmp->parts[tmp->count - 1].data = lex->tok;
		lex_append(lex, &tmp->str, &tmp->len, 1);
		lex_update(lex, 0);
		lex->new_part = 1;
		lex->tok = tmp;
		return (1);
	}
	return (0);
}

int
	lex_special_parameter(t_lexer *lex)
{
	t_token	*tmp;

	if (lex_peculiar_parameter(lex))
		return (1);
	if ((ft_isalnum(lex->src->nex) || lex->src->nex == '_')
		&& !lex->btick)
	{
		lex_update(lex, 1);
		token_add_part(lex, lx_parameter, lex->quote);
		tmp = lex->tok;
		lex->tok = sh_safe_malloc(sizeof(t_token));
		token_init(lex->tok);
		lex->tok->id = tk_word;
		tmp->parts[tmp->count - 1].data = lex->tok;
		while ((ft_isalnum(lex->src->cur) || lex->src->cur == '_')
			&& !lex->btick)
		{
			lex_append(lex, &tmp->str, &tmp->len, 1);
			lex_update(lex, 0);
		}
		lex->new_part = 1;
		lex->tok = tmp;
		return (1);
	}
	return (0);
}

int
	lex_special_dollar(t_lexer *lex)
{
	t_tpart	*part;

	if (lex->src->nex == '(')
		return (lex_special_lparen(lex));
	else if (lex->src->nex == '{')
	{
		lex_update(lex, 1);
		lex_nom_and_skip(lex);
		part = token_add_part(lex, lx_parameter, lex->quote);
		lex_parameter(lex, part);
		if (lex->src->cur != -1)
			lex_update(lex, 1);
		lex->new_part = 1;
		return (1);
	}
	return (lex_special_parameter(lex));
}

int
	lex_special(t_lexer *lex)
{
	if (lex->src->cur == '\'' || lex->src->cur == '"')
	{
		if (!lex->quote)
			lex->empty_quote = 1;
		if (lex->quote && lex->empty_quote)
			token_add_part(lex, lx_normal, lex->quote);
		lex->new_part = 1;
		if (lex->src->cur == '\'')
			lex->quote = 1 - lex->quote;
		else
			lex->quote = 2 - lex->quote;
		lex_update(lex, 1);
		return (1);
	}
	else if (lex->src->cur == '$')
		return (lex_special_dollar(lex));
	else if (lex->src->cur == '`')
	{
		lex_update(lex, 1);
		if (!lex->btick)
			token_add_part(lex, lx_backtick, lex->quote);
		lex->new_part = lex->btick;
		return (lex->btick = !lex->btick, 1);
	}
	return (0);
}
