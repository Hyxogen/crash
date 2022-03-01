/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   command_extra.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:27:18 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/01 11:32:48 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int
	pr_cmd_prefix(t_parser *pr, t_snode *parent, t_snode *red, t_snode *ass)
{
	(void) parent;
	while (pr_io_redirect(pr, red)
		|| (pr->current_ret != 0
			&& pr_convert_ass(pr, pr->current, 1)
			&& pr_token(pr, ass, sx_assword, tk_assword)))
		continue ;
	if (red->childs_size != 0 || ass->childs_size != 0)
		return (1);
	return (0);
}

int
	pr_cmd_suffix(t_parser *pr, t_snode *parent, t_snode *red, t_snode *ass)
{
	t_snode	*node;

	node = snode(sx_cmd_suffix);
	while (pr_io_redirect(pr, red)
		|| pr_token(pr, node, sx_word, tk_word))
		continue ;
	if (node->childs_size != 0)
	{
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	if (red->childs_size != 0 || ass->childs_size != 0)
		return (1);
	return (0);
}
