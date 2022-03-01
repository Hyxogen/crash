/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   case.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:16:12 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/01 14:24:34 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int
	pr_case_item(t_parser *pr, t_snode *parent)
{
	t_token	*token;

	pr_token(pr, NULL, sx_none, op_lparen);
	if (pr_pattern(pr, &token)
		&& pr_token(pr, NULL, sx_none, op_rparen))
	{
		if (!pr_compound_list(pr, parent))
		{
			while (pr_token(pr, NULL, sx_none, tk_newline))
				continue ;
			node_add_child(parent, snode(sx_compound_list));
		}
		parent->childs[parent->childs_size - 1]->token = token;
		if (pr_token(pr, NULL, sx_none, op_dsemi))
		{
			while (pr_token(pr, NULL, sx_none, tk_newline))
				continue ;
			return (1);
		}
	}
	return (0);
}

int
	pr_case_clause(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	if (!pr_token(pr, NULL, sx_none, kw_case))
		return (0);
	node = snode(sx_case_clause);
	if (pr_token_set(pr, node, tk_word))
	{
		while (pr_token(pr, NULL, sx_none, tk_newline))
			continue ;
		if (pr_convert_keyword(pr, pr->current, kw_in)
			&& pr_token(pr, NULL, sx_none, kw_in))
		{
			while (pr_token(pr, NULL, sx_none, tk_newline))
				continue ;
			while (pr_case_item(pr, node))
				continue ;
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
