/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   separator.c                                      -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:23:31 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:23:31 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int
	pr_seperator_op(t_parser *pr, t_snode *parent)
{
	if (pr_token(pr, NULL, sx_and, op_and))
	{
		parent->flags |= flag_and;
		return (1);
	}
	if (pr_token(pr, NULL, sx_semicolon, op_semi))
	{
		parent->flags |= flag_semi;
		return (1);
	}
	return (0);
}

int
	pr_sequential_sep(t_parser *pr, t_snode *parent)
{
	(void) parent;
	if (!pr_token(pr, NULL, sx_none, op_semi)
		&& !pr_token(pr, NULL, sx_none, tk_newline))
		return (0);
	while (pr_token(pr, NULL, sx_newline, tk_newline))
		continue ;
	return (1);
}
