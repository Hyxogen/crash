/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   special.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/22 14:07:12 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/28 10:49:51 by dmeijer       ########   odam.nl         */
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

int
	lex_special_parameter(t_lexer *lex)
{
	t_token	*tmp;
	
	if ((ft_isalnum(lex->src->nex) || lex->src->cur == '_')
		&& !lex->btick)
	{
		lex_update(lex, 1);
		tmp = lex->tok;
		token_add_part(tmp, lx_parameter);
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
		lex->tok = tmp;
		token_add_part(lex->tok, lx_normal);
		return (1);
	}
	return (0);
}

int
	lex_special_dollar(t_lexer *lex)
{
	t_tpart	*part;

	if (lex->src->nex == '(')
	{
		return (lex_special_lparen(lex));
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
	return (lex_special_parameter(lex));
}

int
	lex_special(t_lexer *lex)
{
	if (lex->src->cur == '\'' || lex->src->cur == '"')
	{
		if (lex->src->cur == '\'')
			lex->quote = 1 - lex->quote;
		else
			lex->quote = 2 - lex->quote;
		lex_update(lex, 1);
		return (1);
	}
	else if (lex->src->cur == '$')
	{
		return (lex_special_dollar(lex));
	}
	else if (lex->src->cur == '`')
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
