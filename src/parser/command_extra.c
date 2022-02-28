/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   command_extra.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:27:18 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/28 16:18:09 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int
	pr_cmd_prefix(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_cmd_prefix);
	while (pr_io_redirect(pr, node)
		|| (pr->current_ret != 0
			&& pr_convert_ass(pr, pr->current, 1)
			&& pr_token(pr, node, sx_assword, tk_assword)))
		continue ;
	if (node->childs_size != 0)
	{
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}

int
	pr_cmd_suffix(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_cmd_suffix);
	while (pr_io_redirect(pr, node)
		|| pr_token(pr, node, sx_word, tk_word))
		continue ;
	if (node->childs_size != 0)
	{
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}
