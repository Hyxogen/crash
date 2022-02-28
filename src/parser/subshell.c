/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   subshell.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:19:35 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/28 10:19:44 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int
	pr_subshell(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_subshell);
	if (pr_token(pr, NULL, sx_none, op_lparen))
	{
		if (pr_compound_list(pr, node))
		{
			if (pr_token(pr, NULL, sx_none, op_rparen))
			{
				node_add_child(parent, node);
				return (1);
			}
		}
	}
	node_destroy(node);
	return (0);
}
