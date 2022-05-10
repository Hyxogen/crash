/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   subshell.c                                       -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:34 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:34 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int
	pr_subshell(t_parser *pr, t_snode *parent)
{
	if (pr_token(pr, NULL, sx_none, op_lparen))
	{
		if (pr_compound_list(pr, parent))
		{
			if (pr_token(pr, NULL, sx_none, op_rparen))
			{
				parent->childs[parent->childs_size - 1]->flags |= flag_subshell;
				return (1);
			}
		}
		pr->lexer->error = SH_PR_UNEXTOKEN;
	}
	return (0);
}
