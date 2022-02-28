/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   case.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:16:12 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/28 10:37:25 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int
	pr_case_item(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_case_item);
	pr_token(pr, NULL, sx_none, op_lparen);
	if (pr_pattern(pr, node)
		&& pr_token(pr, NULL, sx_none, op_rparen))
	{
		if (!pr_compound_list(pr, node))
		{
			while (pr_token(pr, NULL, sx_none, tk_newline))
				continue ;
		}
		if (pr_token(pr, NULL, sx_none, op_dsemi))
		{
			while (pr_token(pr, NULL, sx_none, tk_newline))
				continue ;
			node_add_child(parent, node);
			return (1);
		}
	}
	node_destroy(node);
	return (0);
}

int
	pr_case_list(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_case_list);
	if (pr_case_item(pr, node))
	{
		pr_case_list(pr, node);
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}

int
	pr_case_clause(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	if (!pr_token(pr, NULL, sx_none, kw_case))
		return (0);
	node = snode(sx_case_clause);
	if (pr_token(pr, node, sx_word, tk_word))
	{
		while (pr_token(pr, NULL, sx_none, tk_newline))
			continue ;
		if (pr_convert_keyword(pr, pr->current, kw_in)
			&& pr_token(pr, NULL, sx_none, kw_in))
		{
			while (pr_token(pr, NULL, sx_none, tk_newline))
				continue ;
			pr_case_list(pr, node);
			if (pr_token(pr, NULL, sx_none, kw_esac))
			{
				node_add_child(parent, node);
				return (1);
			}
		}
	}
	node_destroy(node);
	return (0);
}
