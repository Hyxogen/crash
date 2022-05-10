/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   expand_arith_tok.c                               -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:59:38 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:59:38 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"

t_arith_op
	*get_binary_operators(void)
{
	static t_arith_op	binary_operators[SH_BINARY_OP_COUNT];

	return (binary_operators);
}

t_arith_op
	*get_unary_operators(void)
{
	static t_arith_op	unary_operators[SH_UNARY_OP_COUNT] = {
	{ar_op_plus, 100 - 2, ass_right, arith_identity, 0},
	{ar_op_minus, 100 - 2, ass_right, arith_negate, 0},
	{ar_op_excl, 100 - 2, ass_right, arith_logical_not, 0},
	{ar_op_tilde, 100 - 2, ass_right, arith_bitwise_not, 0}
	};

	return (unary_operators);
}

t_arith_optok
	*get_operator_tokens(void)
{
	static t_arith_optok	operator_tokens[SH_OP_TOK_COUNT];

	return (operator_tokens);
}
