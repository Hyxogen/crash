/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   expand_arith_optok_init.c                        -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:59:37 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:59:37 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"

static void
	arith_set_optok(size_t index, const char *str, t_arith_token_id id)
{
	get_operator_tokens()[index].str = str;
	get_operator_tokens()[index].id = id;
}

static void
	arith_optok_init1(void)
{
	arith_set_optok(0, "<", ar_op_less);
	arith_set_optok(1, "<=", ar_op_less_eq);
	arith_set_optok(2, "<<", ar_op_less_less);
	arith_set_optok(3, "<<=", ar_op_less_less_eq);
	arith_set_optok(4, ">", ar_op_great);
	arith_set_optok(5, ">=", ar_op_great_eq);
	arith_set_optok(6, ">>", ar_op_great_great);
	arith_set_optok(7, ">>=", ar_op_great_great_eq);
	arith_set_optok(8, "&", ar_op_amp);
	arith_set_optok(9, "&&", ar_op_amp_amp);
	arith_set_optok(10, "&=", ar_op_amp_eq);
	arith_set_optok(11, "|", ar_op_pipe);
	arith_set_optok(12, "||", ar_op_pipe_pipe);
	arith_set_optok(13, "|=", ar_op_pipe_eq);
	arith_set_optok(14, "+", ar_op_plus);
	arith_set_optok(15, "+=", ar_op_plus_eq);
	arith_set_optok(16, "-", ar_op_minus);
	arith_set_optok(17, "-=", ar_op_minus_eq);
	arith_set_optok(18, "*", ar_op_star);
	arith_set_optok(19, "*=", ar_op_star_eq);
	arith_set_optok(20, "/", ar_op_slash);
	arith_set_optok(21, "/=", ar_op_slash_eq);
	arith_set_optok(22, "%", ar_op_perc);
	arith_set_optok(23, "%=", ar_op_perc_eq);
	arith_set_optok(24, "^", ar_op_caret);
}

static void
	arith_optok_init2(void)
{
	arith_set_optok(25, "^=", ar_op_caret_eq);
	arith_set_optok(26, "(", ar_op_lparen);
	arith_set_optok(27, ")", ar_op_rparen);
	arith_set_optok(28, "?", ar_op_quest);
	arith_set_optok(29, ":", ar_op_colon);
	arith_set_optok(30, "=", ar_op_eq);
	arith_set_optok(31, "==", ar_op_eq_eq);
	arith_set_optok(32, "!=", ar_op_excl_eq);
	arith_set_optok(33, "!", ar_op_excl);
	arith_set_optok(34, "~", ar_op_tilde);
}

void
	arith_optok_init(void)
{
	arith_optok_init1();
	arith_optok_init2();
}
