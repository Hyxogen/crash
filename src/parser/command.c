/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   command.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:05:14 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/24 10:23:20 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

int
	pr_compound_cmd(t_parser *pr, t_snode *parent)
{
	t_snode	*red;

	if (pr->current.id != tk_invalid)
	{
		pr_convert_reserved(pr, &pr->current);
		if (pr_brace_group(pr, parent)
			|| pr_subshell(pr, parent)
			|| pr_while_clause(pr, parent)
			|| pr_until_clause(pr, parent)
			|| pr_if_clause(pr, parent)
			|| pr_for_clause(pr, parent)
			|| pr_case_clause(pr, parent))
		{
			red = snode(sx_io_redirect_list);
			pr_redirect_list(pr, red);
			node_add_child(parent, red);
			return (1);
		}
	}
	return (0);
}

int
	pr_cmd(t_parser *pr, t_snode *parent)
{
	if (pr_function_def(pr, parent)
		|| pr_compound_cmd(pr, parent)
		|| pr_simple_cmd(pr, parent))
		return (1);
	return (0);
}

int
	pr_complete_cmd(t_parser *pr, t_snode *parent)
{
	if (pr->current.id == tk_invalid)
		return (0);
	if (pr_list(pr, parent))
	{
		if (pr->next.id == tk_invalid)
			return (1);
	}
	return (0);
}

int
	pr_complete_cmdlst(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_command_list);
	while (pr_complete_cmd(pr, node))
	{
		token_destroy(&pr->current);
		pr_next_token(pr);
	}
	node_add_child(parent, node);
	return (1);
}

int
	pr_simple_cmd(t_parser *pr, t_snode *parent)
{
	t_snode	*node;
	t_snode	*red;
	t_snode	*ass;
	t_snode	*cmd;

	red = snode(sx_io_redirect_list);
	ass = snode(sx_ass_list);
	cmd = snode(sx_wordlist);
	if (pr_cmd_prefix(pr, red, ass, cmd))
		pr_token(pr, cmd, sx_word, tk_word);
	else if (!pr_token(pr, cmd, sx_word, tk_word))
	{
		node_destroy(red);
		node_destroy(ass);
		node_destroy(cmd);
		return (0);
	}
	pr_cmd_suffix(pr, red, ass, cmd);
	node = snode(sx_simple_cmd);
	node_add_child(node, cmd);
	node_add_child(node, red);
	node_add_child(node, ass);
	node_add_child(parent, node);
	return (1);
}
