/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   expand_arith_bin2.c                              -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:59:34 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:59:34 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"

t_arith_value
	arith_parse_binary(t_arith_parser *pr,
		t_arith_value lhs, int min_prec)
{
	t_arith_op			*lookahead;
	const t_arith_op	*op;

	lookahead = arith_get_binary_op(pr->next.id);
	while (lookahead != NULL && lookahead->prec >= min_prec)
	{
		op = lookahead;
		arith_lex(pr->lex, &pr->next);
		if (op->token == ar_op_quest)
			arith_binary_handle_quest(pr, &lhs, op, &lookahead);
		else
			arith_binary_handle_notquest(pr, &lhs, op, &lookahead);
		lhs.str = NULL;
		lhs.size = 0;
	}
	return (lhs);
}
