/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   and_or.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:05:59 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/28 11:22:41 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*Todo test if something like "echo Hallo && || cmd" doesn't work*/
int
	pr_and_or(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_and_or);
	if (pr_pipeline(pr, node))
	{
		if (pr_token(pr, NULL, sx_and_if, op_andif))
			node->flags |= flag_and_if;
		else if (pr_token(pr, NULL, sx_or_if, op_orif))
			node->flags |= flag_or_if;
		if (node->flags)
		{
			while (pr_token(pr, NULL, sx_newline, tk_newline))
				continue ;
			if (!pr_and_or(pr, node))
			{
				node_destroy(node);
				return (0);
			}
		}
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}
