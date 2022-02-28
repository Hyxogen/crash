/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   function.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:16:44 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/28 11:27:15 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int
	pr_convert_func_def(t_parser *pr)
{
	if (pr->next->id == op_lparen)
		if (pr_convert_name(pr, pr->current))
			return (1);
	return (0);
}

int
	pr_function_body(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_function_body);
	if (pr_compound_cmd(pr, node))
	{
		pr_redirect_list(pr, node);
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}

int
	pr_function_def(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	if (!pr_convert_func_def(pr))
	{
		
		return (0);
	}
		

	node = snode(sx_function_def);
	pr_token(pr, node, sx_function_name, tk_name);
	pr_token(pr, NULL, sx_none, op_lparen);
	if (pr_token(pr, NULL, sx_none, op_rparen))
	{
		
		while (pr_token(pr, NULL, sx_none, tk_newline))
			continue ;
		if (pr_function_body(pr, node))
		{
			
			node_add_child(parent, node);
			return (1);
		}
	}
	node_destroy(node);
	return (0);
}
