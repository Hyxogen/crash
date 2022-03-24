/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipe.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:06:29 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/24 13:57:04 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int
	pr_pipe_sequence(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_pipe_sequence);
	while (1)
	{
		if (pr_cmd(pr, node))
		{
			if (!pr_token(pr, NULL, sx_pipe, op_pipe))
				break ;
			while (pr_token(pr, NULL, sx_newline, tk_newline))
				continue ;
		}
		else
		{
			node_destroy(node);
			return (0);
		}
	}
	if (node->childs_size == 0)
	{
		node_destroy(node);
		return (0);
	}
	node_add_child(parent, node);
	return (1);
}

int
	pr_pipeline(t_parser *pr, t_snode *parent)
{
	if (pr->current.id != tk_invalid
		&& pr_convert_reserved(pr, &pr->current)
		&& pr_token(pr, NULL, sx_none, kw_bang))
	{
		if (pr_pipe_sequence(pr, parent))
		{
			parent->childs[parent->childs_size - 1]->flags |= flag_bang;
			return (1);
		}
		else
		{
			pr->lexer->error = SH_PR_UNEXTOKEN;
			return (0);
		}
	}
	return (pr_pipe_sequence(pr, parent));
}
