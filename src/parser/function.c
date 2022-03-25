/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   function.c                                       -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:23:13 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:23:13 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int
	pr_convert_func_def(t_parser *pr)
{
	if (pr->next.id == op_lparen)
		if (pr->current.id != tk_invalid && pr_convert_name(pr, &pr->current))
			return (1);
	return (0);
}

int
	pr_function_def(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	if (!pr_convert_func_def(pr))
		return (0);
	node = snode(sx_function_def);
	pr_token_set(pr, node, tk_name);
	pr_error_token(pr, NULL, sx_none, op_lparen);
	if (pr_error_token(pr, NULL, sx_none, op_rparen))
	{
		while (pr_token(pr, NULL, sx_none, tk_newline))
			continue ;
		if (pr_compound_cmd(pr, node))
		{
			node_add_child(parent, node);
			return (1);
		}
		pr->lexer->error = SH_PR_UNEXTOKEN;
	}
	node_destroy(node);
	return (0);
}
