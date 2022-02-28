/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   condition.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:12:45 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/28 16:17:53 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int
	pr_elif_part(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_elif_part);
	if (pr_token(pr, NULL, sx_none, kw_elif)
		&& pr_condition(pr, node)
		&& pr_token(pr, NULL, sx_none, kw_then)
		&& pr_compound_list(pr, node))
	{
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}

int
	pr_else_part(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_else_part);
	if (pr_token(pr, NULL, sx_none, kw_else)
		&& pr_compound_list(pr, node))
	{
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}

int
	pr_if_clause(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_if_clause);
	if (pr_token(pr, NULL, sx_none, kw_if)
		&& pr_condition(pr, node)
		&& pr_token(pr, NULL, sx_none, kw_then)
		&& pr_compound_list(pr, node))
	{
		while (pr_elif_part(pr, node))
			continue ;
		pr_else_part(pr, node);
		if (pr_token(pr, NULL, sx_none, kw_fi))
		{
			node_add_child(parent, node);
			return (1);
		}
	}
	node_destroy(node);
	return (0);
}

int
	pr_condition(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_condition);
	if (pr_compound_list(pr, node))
	{
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}
