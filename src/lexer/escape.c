/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   escape.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 16:04:47 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/28 16:04:48 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int
	lex_quoted(t_lexer *lex)
{
	if (lex->src->bslash)
		return (1);
	if (lex->btick)
	{
		if (lex->src->cur != '\\' && lex->src->cur != '`')
			return (1);
		return (0);
	}
	if (lex->quote == 1 && lex->src->cur != '\'')
		return (1);
	if (lex->quote == 2)
	{
		if (lex->src->cur == '$')
			return (0);
		if (lex->src->cur == '`')
			return (0);
		if (lex->src->cur == '\\')
			return (0);
		if (lex->src->cur == '"')
			return (0);
		return (1);
	}
	return (0);
}

int
	lex_bquoted(t_lexer *lex)
{
	if (lex->quote || lex->btick || lex->end != NULL)
	{
		if (lex->src->cur == '$')
			return (1);
		if (lex->src->cur == '`')
			return (1);
		if (lex->src->cur == '"' && !lex->btick && lex->end == NULL)
			return (1);
		if (lex->src->cur == '\\')
			return (1);
		return (0);
	}
	return (1);
}
