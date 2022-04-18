/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   command.c                                        -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:23:02 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:23:02 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

int
	pr_compound_cmd(t_parser *pr, t_snode *parent)
{
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
			if (!pr_redirect_list(pr, parent->childs[parent->childs_size - 1]))
				node_add_child(parent->childs[parent->childs_size - 1], snode(sx_io_redirect_list));
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
	if (pr->current.id != tk_newline)
		pr->lexer->error = SH_PR_UNEXTOKEN;
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
