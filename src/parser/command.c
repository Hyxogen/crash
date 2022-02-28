/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   command.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:05:14 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/28 15:34:27 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int
	pr_compound_cmd(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_compound_cmd);
	if (pr->current_ret)
	{
		pr_convert_reserved(pr, pr->current);
		if (pr_brace_group(pr, node)
			|| pr_subshell(pr, node)
			|| pr_while_clause(pr, node)
			|| pr_until_clause(pr, node)
			|| pr_if_clause(pr, node)
			|| pr_for_clause(pr, node)
			|| pr_case_clause(pr, node))
		{
			node_add_child(parent, node);
			return (1);
		}
	}
	node_destroy(node);
	return (0);
}

int
	pr_cmd(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_cmd);
	if (pr_function_def(pr, node))
		;
	else if (pr_compound_cmd(pr, node))
		pr_redirect_list(pr, node);
	else if (pr_simple_cmd(pr, node))
		;
	else
	{
		node_destroy(node);
		return (0);
	}
	node_add_child(parent, node);
	return (1);
}

int
	pr_complete_cmd(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	if (!pr->current_ret)
		return (0);
	node = snode(sx_complete_cmd);
	if (pr_list(pr, node))
	{
		if (!pr->next_ret)
		{
			node_add_child(parent, node);
			return (1);
		}
	}
	node_destroy(node);
	return (0);
}

int
	pr_complete_cmdlst(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_complete_cmdlst);
	while (pr->current_ret != -1 && pr_complete_cmd(pr, node))
	{
		pr_next_token(pr);
		continue ;
	}
	node_add_child(parent, node);
	return (1);
}

int
	pr_simple_cmd(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_simple_cmd);
	if (pr_cmd_prefix(pr, node))
		pr_token(pr, node, sx_cmd_word, tk_word);
	else if (!pr_token(pr, node, sx_cmd_word, tk_word))
	{
		node_destroy(node);
		return (0);
	}
	pr_cmd_suffix(pr, node);
	node_add_child(parent, node);
	return (1);
}
