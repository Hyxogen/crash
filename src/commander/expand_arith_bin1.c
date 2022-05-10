/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   expand_arith_bin1.c                              -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:59:33 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:59:33 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"

#include <stdlib.h>

void
	arith_binary_handle_quest(t_arith_parser *pr,
		t_arith_value *lhs, const t_arith_op *op, t_arith_op **lookahead)
{
	t_arith_value		rhs;
	t_arith_value		super_rhs;

	pr->is_fake += !lhs->value;
	rhs = arith_parse_unary(pr);
	rhs = arith_parse_binary(pr, rhs, 0);
	pr->is_fake -= !lhs->value;
	if (pr->next.id == ar_op_colon)
	{
		arith_lex(pr->lex, &pr->next);
		pr->is_fake += !!lhs->value;
		super_rhs = arith_parse_unary(pr);
		super_rhs = arith_parse_binary(pr, super_rhs, 100 - 13);
		pr->is_fake -= !!lhs->value;
		if (!pr->is_fake || pr->error)
			lhs->value = op->proc(NULL, lhs->value, rhs.value, super_rhs.value);
		*lookahead = arith_get_binary_op(pr->next.id);
	}
	else
		pr->error = 1;
}

static void
	arith_binary_handle_notquest_ampamp(t_arith_parser *pr,
		t_arith_value *lhs, t_arith_value *rhs, t_arith_op **lookahead)
{
	pr->is_fake += !lhs->value;
	*rhs = arith_parse_unary(pr);
	*rhs = arith_parse_binary(pr, *rhs, 100 - 12);
	pr->is_fake -= !lhs->value;
	*lookahead = arith_get_binary_op(pr->next.id);
}

static void
	arith_binary_handle_notquest_pipepipe(t_arith_parser *pr,
		t_arith_value *lhs, t_arith_value *rhs, t_arith_op **lookahead)
{
	pr->is_fake += !!lhs->value;
	*rhs = arith_parse_unary(pr);
	*rhs = arith_parse_binary(pr, *rhs, 100 - 13);
	pr->is_fake -= !!lhs->value;
	*lookahead = arith_get_binary_op(pr->next.id);
}

static void
	arith_inary_handle_notquest_nofake(t_arith_parser *pr,
		t_arith_value *lhs, t_arith_value *rhs, const t_arith_op *op)
{
	const char			*tmp;

	(void) pr;
	tmp = ft_strndup(lhs->str, lhs->size);
	lhs->value = op->proc(tmp, lhs->value, rhs->value, 0);
	free((void *) tmp);
}

void
	arith_binary_handle_notquest(t_arith_parser *pr,
		t_arith_value *lhs, const t_arith_op *op, t_arith_op **lookahead)
{
	t_arith_value		rhs;

	if (op->token == ar_op_amp_amp)
		arith_binary_handle_notquest_ampamp(pr, lhs, &rhs, lookahead);
	else if (op->token == ar_op_pipe_pipe)
		arith_binary_handle_notquest_pipepipe(pr, lhs, &rhs, lookahead);
	else
	{
		rhs = arith_parse_unary(pr);
		*lookahead = arith_get_binary_op(pr->next.id);
		while (*lookahead != NULL && ((*lookahead)->prec > op->prec
				|| ((*lookahead)->ass == ass_right
					&& (*lookahead)->prec == op->prec)))
		{
			rhs = arith_parse_binary(pr, rhs,
					op->prec + ((*lookahead)->prec > op->prec));
			*lookahead = arith_get_binary_op(pr->next.id);
		}
	}
	if (op->is_assignment && lhs->str == NULL)
		pr->error = 1;
	else if (!pr->is_fake || pr->error)
		arith_inary_handle_notquest_nofake(pr, lhs, &rhs, op);
}
