/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lex_command.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/22 10:53:06 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/22 11:10:28 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "new_lexer.h"


void
	lex_command(t_lexer *lex, t_tpart *part)
{
	t_lexer		cmd_lex;
	t_parser	cmd_pr;
	t_snode		*node;

	lex_init(&cmd_lex);
	cmd_lex.prev = lex;
	cmd_lex.id = lx_command;
	cmd_lex.src = lex->src;
	cmd_lex.end = NULL;
	cmd_lex.trim = 0;
	pr_init(&cmd_pr);
	cmd_pr.lexer = &cmd_lex;
	node = pr_parse(&cmd_pr);
	part->data = node;
	part->quote = lex->quote;
}