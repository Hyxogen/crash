/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lex_btick.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/22 10:53:26 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/22 11:16:19 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "new_lexer.h"

static const char
	*_lex_capture(t_lexer *lex)
{
	const char	*beg;

	while (1)
	{

	}
}

void
	lex_btick(t_lexer *lex, t_tpart *part)
{
	t_lexer	b_lex;

	lex_init(&b_lex);
	b_lex.prev = lex;
	b_lex.id = lx_normal;
	b_lex.src = lex->src;
	b_lex.end = NULL;
	b_lex.trim = 0;
}