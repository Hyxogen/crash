/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   expand_arith_unary.c                             -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:59:42 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:59:42 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"

#include <stdlib.h>

static int
	arith_parse_not_null(t_arith_parser *pr,
		const t_arith_op *op, t_arith_value *expr)
{
	if (op != NULL)
	{
		arith_lex(pr->lex, &pr->next);
		*expr = arith_parse_unary(pr);
		if (!pr->is_fake || pr->error)
		{
			expr->value = op->proc(NULL, expr->value, 0, 0);
		}
		return (1);
	}
	return (0);
}

static int
	arith_parse_next_lparen(t_arith_parser *pr,
		const t_arith_op *op, t_arith_value *expr)
{
	(void) op;
	if (pr->next.id == ar_op_lparen)
	{
		arith_lex(pr->lex, &pr->next);
		*expr = arith_parse_unary(pr);
		*expr = arith_parse_binary(pr, *expr, 0);
		if (pr->next.id == ar_op_rparen)
			arith_lex(pr->lex, &pr->next);
		else
			pr->error = 1;
		return (1);
	}
	return (0);
}

static int
	arith_parse_next_const(t_arith_parser *pr,
		const t_arith_op *op, t_arith_value *expr)
{
	const char				*key;

	(void) op;
	if (pr->next.id == ar_tk_const)
	{
		key = ft_strndup(pr->next.str, pr->next.size);
		if (sh_arith_atol(key, &expr->value) < 0)
			pr->error = 1;
		free((void *) key);
		arith_lex(pr->lex, &pr->next);
		return (1);
	}
	return (0);
}

static int
	arith_parse_next_ident(t_arith_parser *pr,
		const t_arith_op *op, t_arith_value *expr)
{
	const char				*key;
	const char				*tmp;

	(void) op;
	if (pr->next.id == ar_tk_ident)
	{
		key = ft_strndup(pr->next.str, pr->next.size);
		tmp = sh_getenv(key, "0");
		free((void *) key);
		expr->value = expand_arith_str(tmp,
				&pr->error, pr->recursion_level + 1);
		expr->str = pr->next.str;
		expr->size = pr->next.size;
		arith_lex(pr->lex, &pr->next);
		return (1);
	}
	return (0);
}

t_arith_value
	arith_parse_unary(t_arith_parser *pr)
{
	const t_arith_op		*op;
	t_arith_value			expr;

	op = arith_get_unary_op(pr->next.id);
	expr.value = 0;
	expr.str = NULL;
	expr.size = 0;
	if (arith_parse_not_null(pr, op, &expr)
		|| arith_parse_next_lparen(pr, op, &expr)
		|| arith_parse_next_const(pr, op, &expr)
		|| arith_parse_next_ident(pr, op, &expr))
		return (expr);
	pr->error = 1;
	return (expr);
}
