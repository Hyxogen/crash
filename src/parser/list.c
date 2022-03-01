/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   list.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:05:42 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/01 15:01:51 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int
	pr_compound_list(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_compound_list);
	while (pr_token(pr, NULL, sx_newline, tk_newline))
		continue ;
	if (pr_term(pr, node))
	{
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}

int
	pr_list(t_parser *pr, t_snode *parent)
{
	t_snode	*node;
	t_snode	*term;

	node = snode(sx_list);
	term = snode(sx_term);
	while (pr_and_or(pr, term))
	{
		if (pr_token(pr, NULL, sx_and, op_and))
			term->flags |= flag_and;
		else if (pr_token(pr, NULL, sx_semicolon, op_semi))
			term->flags |= flag_semi;
		node_add_child(node, term);
		term = snode(sx_term);
	}
	node_destroy(term);
	if (node->childs_size == 0)
	{
		node_destroy(node);
		return (0);
	}
	node_add_child(parent, node);
	return (1);
}
