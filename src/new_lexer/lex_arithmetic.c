/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lex_arithmetic.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/22 10:53:42 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/22 11:11:16 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "new_lexer.h"
#include "memory.h"

void
	lex_arithmetic(t_lexer *lex, t_tpart *part)
{
	t_lexer	lexer;

	part->id = lx_arithmetic;
	part->data = sh_safe_malloc(sizeof(t_token));
	part->quote = lex->quote;
	lex_init(&lexer);
	lexer.tok = part->data;
	lexer.prev = lex;
	lexer.id = lx_arithmetic;
	lexer.src = lex->src;
	lexer.end = NULL;
	lexer.trim = 0;
	lex_main(&lexer);
	/* TODO: destroy resources */
}