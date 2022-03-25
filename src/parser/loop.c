/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   loop.c                                           -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:23:21 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:23:21 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int
	pr_for_clause(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_for_clause);
	if (pr_token(pr, NULL, sx_none, kw_for))
	{
		if (pr_convert_name(pr, &pr->current)
			&& pr_token_set(pr, node, tk_name))
		{
			while (pr_token(pr, NULL, sx_newline, tk_newline))
				continue ;
			if (!pr_convert_reserved(pr, &pr->current)
				|| !pr_token(pr, NULL, sx_none, kw_in)
				|| !pr_wordlist(pr, node)
				|| !pr_sequential_sep(pr, node))
				node_add_child(node, snode(sx_wordlist));
			if (pr_do_group(pr, node))
			{
				node_add_child(parent, node);
				return (1);
			}
		}
		pr->lexer->error = SH_PR_UNEXTOKEN;
	}
	node_destroy(node);
	return (0);
}

int
	pr_while_clause(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_while_clause);
	if (pr_token(pr, NULL, sx_none, kw_while))
	{
		if (pr_compound_list(pr, node)
			&& pr_do_group(pr, node))
		{
			node_add_child(parent, node);
			return (1);
		}
		pr->lexer->error = SH_PR_UNEXTOKEN;
	}
	node_destroy(node);
	return (0);
}

int
	pr_until_clause(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_while_clause);
	node->flags |= flag_until;
	if (pr_token(pr, NULL, sx_none, kw_until))
	{
		if (pr_compound_list(pr, node)
			&& pr_do_group(pr, node))
		{
			node_add_child(parent, node);
			return (1);
		}
		pr->lexer->error = SH_PR_UNEXTOKEN;
	}
	node_destroy(node);
	return (0);
}

int
	pr_do_group(t_parser *pr, t_snode *parent)
{
	if (pr_convert_reserved(pr, &pr->current)
		&& pr_token(pr, NULL, sx_none, kw_do))
	{
		if (pr_compound_list(pr, parent)
			&& pr_token(pr, NULL, sx_none, kw_done))
			return (1);
		pr->lexer->error = SH_PR_UNEXTOKEN;
	}
	return (0);
}
