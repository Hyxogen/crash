/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipe.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:06:29 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/28 11:24:12 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int
	pr_pipe_sequence(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_pipe_sequence);
	if (pr_cmd(pr, node))
	{
		if (pr_token(pr, NULL, sx_pipe, op_pipe))
		{
			while (pr_token(pr, NULL, sx_newline, tk_newline))
				continue ;
			if (!pr_pipe_sequence(pr, node))
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

int
	pr_pipeline(t_parser *pr, t_snode *parent)
{
	if (pr->current_ret != 0
		&& pr_convert_reserved(pr, pr->current)
		&& pr_bang(pr, parent))
		return (1);
	return (pr_pipe_sequence(pr, parent));
}
