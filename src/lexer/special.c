/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   special.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/22 14:07:12 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/25 10:42:16 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

#include "memory.h"

// lx_normal: abc
// lx_command: echo 1
// lx_normal: def

// abc$(echo 1)def

int
	lex_special_dollar(t_lexer *lex)
{
	t_tpart	*part;

	if (lex->src->nex == '(')
	{
		lex_update(lex, 1);
		if (lex->src->nex == '(')
		{
			lex_update(lex, 1);
			lex_nom_and_skip(lex);
			part = token_add_part(lex->tok, lx_arithmetic);
			lex_arithmetic(lex, part);
			lex_update(lex, 1);
		}
		else
		{
			lex_nom_and_skip(lex);
			part = token_add_part(lex->tok, lx_command);
			lex_command(lex, part);
		}
		lex_update(lex, 1);
		part = token_add_part(lex->tok, lx_normal);
		return (1);
	}
	else if (lex->src->nex == '{')
	{
		lex_update(lex, 1);
		lex_nom_and_skip(lex);
		part = token_add_part(lex->tok, lx_parameter);
		lex_parameter(lex, part);
		lex_update(lex, 1);
		part = token_add_part(lex->tok, lx_normal);
		return (1);
	}
	return (0);
}

int
	lex_special(t_lexer *lex)
{
	if (lex->src->cur == '\'')
	{
		lex->quote = 1 - lex->quote;
		lex_update(lex, 1);
		return (1);
	}
	if (lex->src->cur == '"')
	{
		lex->quote = 2 - lex->quote;
		lex_update(lex, 1);
		return (1);
	}
	/* TODO bracket-less parameter substitutions */
	if (lex->src->cur == '$')
		return (lex_special_dollar(lex));
	if (lex->src->cur == '`')
	{
		lex_update(lex, 1);
		if (lex->btick)
			token_add_part(lex->tok, lx_normal);
		else
			token_add_part(lex->tok, lx_backtick);
		lex->btick = !lex->btick;
		return (1);
	}
	return (0);
}
