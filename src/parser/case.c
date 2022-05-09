/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   case.c                                           -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:23:01 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:23:01 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/* should_array may be any value, it is only meant for norminette */
int
	pr_case_item(t_parser *pr, t_snode *parent, int should_error)
{
	t_token	token;

	token_init(&token);
	should_error = pr_token(pr, NULL, sx_none, op_lparen);
	if (pr_pattern(pr, &token)
		&& pr_error_token(pr, NULL, sx_none, op_rparen))
	{
		if (!pr_compound_list(pr, parent))
		{
			while (pr_token(pr, NULL, sx_none, tk_newline))
				continue ;
			node_add_child(parent, snode(sx_compound_list));
		}
		token_move(&parent->childs[parent->childs_size - 1]->token, &token);
		if (pr_error_token(pr, NULL, sx_none, op_dsemi))
		{
			while (pr_token(pr, NULL, sx_none, tk_newline))
				continue ;
			return (token_destroy(&token), 1);
		}
		should_error = 1;
	}
	if (should_error)
		pr->lexer->error = SH_PR_UNEXTOKEN;
	return (token_destroy(&token), 0);
}

int
	pr_case_clause(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	if (!pr_token(pr, NULL, sx_none, kw_case))
		return (0);
	node = snode(sx_case_clause);
	if (pr_token_set(pr, node, tk_word))
	{
		while (pr_token(pr, NULL, sx_none, tk_newline))
			continue ;
		if (pr_error_convert_keyword(pr, &pr->current, kw_in)
			&& pr_error_token(pr, NULL, sx_none, kw_in))
		{
			while (pr_token(pr, NULL, sx_none, tk_newline))
				continue ;
			while (pr_case_item(pr, node, 0))
				continue ;
			if (pr_error_token(pr, NULL, sx_none, kw_esac))
			{
				node_add_child(parent, node);
				return (1);
			}
		}
	}
	node_destroy(node);
	return (0);
}
