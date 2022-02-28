/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   list.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:05:42 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/28 11:27:30 by dmeijer       ########   odam.nl         */
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

	node = snode(sx_list);
	if (pr_and_or(pr, node))
	{
		if (pr_token(pr, NULL, sx_and, op_and))
			node->flags |= flag_and;
		else if (pr_token(pr, NULL, sx_semicolon, op_semi))
			node->flags |= flag_semi;
		pr_list(pr, node);
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}
