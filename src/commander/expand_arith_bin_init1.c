/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   expand_arith_bin_init1.c                         -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:59:35 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:59:35 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"

void
	arith_binary_init1(void)
{
	get_binary_operators()[0].token = ar_op_star;
	get_binary_operators()[0].prec = 100 - 3;
	get_binary_operators()[0].ass = ass_left;
	get_binary_operators()[0].proc = arith_multiply;
	get_binary_operators()[0].is_assignment = 0;
	get_binary_operators()[1].token = ar_op_slash;
	get_binary_operators()[1].prec = 100 - 3;
	get_binary_operators()[1].ass = ass_left;
	get_binary_operators()[1].proc = arith_divide;
	get_binary_operators()[1].is_assignment = 0;
	get_binary_operators()[2].token = ar_op_perc;
	get_binary_operators()[2].prec = 100 - 3;
	get_binary_operators()[2].ass = ass_left;
	get_binary_operators()[2].proc = arith_modulo;
	get_binary_operators()[2].is_assignment = 0;
	get_binary_operators()[3].token = ar_op_plus;
	get_binary_operators()[3].prec = 100 - 4;
	get_binary_operators()[3].ass = ass_left;
	get_binary_operators()[3].proc = arith_plus;
	get_binary_operators()[3].is_assignment = 0;
	get_binary_operators()[4].token = ar_op_minus;
	get_binary_operators()[4].prec = 100 - 4;
	get_binary_operators()[4].ass = ass_left;
	get_binary_operators()[4].proc = arith_minus;
	get_binary_operators()[4].is_assignment = 0;
}

void
	arith_binary_init2(void)
{
	get_binary_operators()[5].token = ar_op_less_less;
	get_binary_operators()[5].prec = 100 - 5;
	get_binary_operators()[5].ass = ass_left;
	get_binary_operators()[5].proc = arith_shift_left;
	get_binary_operators()[5].is_assignment = 0;
	get_binary_operators()[6].token = ar_op_less;
	get_binary_operators()[6].prec = 100 - 6;
	get_binary_operators()[6].ass = ass_left;
	get_binary_operators()[6].proc = arith_less_than;
	get_binary_operators()[6].is_assignment = 0;
	get_binary_operators()[7].token = ar_op_great_great;
	get_binary_operators()[7].prec = 100 - 5;
	get_binary_operators()[7].ass = ass_left;
	get_binary_operators()[7].proc = arith_shift_right;
	get_binary_operators()[7].is_assignment = 0;
	get_binary_operators()[8].token = ar_op_less_eq;
	get_binary_operators()[8].prec = 100 - 6;
	get_binary_operators()[8].ass = ass_left;
	get_binary_operators()[8].proc = arith_less_than_or_eq;
	get_binary_operators()[8].is_assignment = 0;
	get_binary_operators()[9].token = ar_op_great;
	get_binary_operators()[9].prec = 100 - 6;
	get_binary_operators()[9].ass = ass_left;
	get_binary_operators()[9].proc = arith_greater_than;
	get_binary_operators()[9].is_assignment = 0;
}

void
	arith_binary_init3(void)
{
	get_binary_operators()[10].token = ar_op_great_eq;
	get_binary_operators()[10].prec = 100 - 6;
	get_binary_operators()[10].ass = ass_left;
	get_binary_operators()[10].proc = arith_greater_than_or_eq;
	get_binary_operators()[10].is_assignment = 0;
	get_binary_operators()[11].token = ar_op_eq_eq;
	get_binary_operators()[11].prec = 100 - 7;
	get_binary_operators()[11].ass = ass_left;
	get_binary_operators()[11].proc = arith_equal;
	get_binary_operators()[11].is_assignment = 0;
	get_binary_operators()[12].token = ar_op_excl_eq;
	get_binary_operators()[12].prec = 100 - 7;
	get_binary_operators()[12].ass = ass_left;
	get_binary_operators()[12].proc = arith_not_equal;
	get_binary_operators()[12].is_assignment = 0;
	get_binary_operators()[13].token = ar_op_amp;
	get_binary_operators()[13].prec = 100 - 8;
	get_binary_operators()[13].ass = ass_left;
	get_binary_operators()[13].proc = arith_bitwise_and;
	get_binary_operators()[13].is_assignment = 0;
	get_binary_operators()[14].token = ar_op_caret;
	get_binary_operators()[14].prec = 100 - 9;
	get_binary_operators()[14].ass = ass_left;
	get_binary_operators()[14].proc = arith_bitwise_xor;
	get_binary_operators()[14].is_assignment = 0;
}

void
	arith_binary_init4(void)
{
	get_binary_operators()[15].token = ar_op_pipe;
	get_binary_operators()[15].prec = 100 - 10;
	get_binary_operators()[15].ass = ass_left;
	get_binary_operators()[15].proc = arith_bitwise_or;
	get_binary_operators()[15].is_assignment = 0;
	get_binary_operators()[16].token = ar_op_amp_amp;
	get_binary_operators()[16].prec = 100 - 11;
	get_binary_operators()[16].ass = ass_left;
	get_binary_operators()[16].proc = arith_logical_and;
	get_binary_operators()[16].is_assignment = 0;
	get_binary_operators()[17].token = ar_op_pipe_pipe;
	get_binary_operators()[17].prec = 100 - 12;
	get_binary_operators()[17].ass = ass_left;
	get_binary_operators()[17].proc = arith_logical_or;
	get_binary_operators()[17].is_assignment = 0;
	get_binary_operators()[18].token = ar_op_quest;
	get_binary_operators()[18].prec = 100 - 13;
	get_binary_operators()[18].ass = ass_right;
	get_binary_operators()[18].proc = arith_ternary;
	get_binary_operators()[18].is_assignment = 0;
	get_binary_operators()[19].token = ar_op_eq;
	get_binary_operators()[19].prec = 100 - 14;
	get_binary_operators()[19].ass = ass_right;
	get_binary_operators()[19].proc = arith_assign;
	get_binary_operators()[19].is_assignment = 1;
}

void
	arith_binary_init(void)
{
	arith_binary_init1();
	arith_binary_init2();
	arith_binary_init3();
	arith_binary_init4();
	arith_binary_init5();
	arith_binary_init6();
}
