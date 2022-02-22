/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lex_normal.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/22 10:52:20 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/22 11:01:28 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "new_lexer.h"

void
	lex_normal(t_source *src, t_snode *node)
{
	t_parser	parser;
	t_lexer		lexer;

	lex_init(&lexer);
	lexer.prev = NULL;
	lexer.id = lx_normal;
	lexer.src = src;
	lexer.end = NULL;
	lexer.trim = 0;
	pr_init(&parser);
	*node = *pr_parse(&parser);
	/* TODO: destroy resources */
}