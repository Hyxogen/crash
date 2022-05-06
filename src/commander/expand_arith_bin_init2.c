#include "commander.h"

void
	arith_binary_init5(void)
{
	get_binary_operators()[20].token = ar_op_plus_eq;
	get_binary_operators()[20].prec = 100 - 14;
	get_binary_operators()[20].ass = ass_right;
	get_binary_operators()[20].proc = arith_plus_eq;
	get_binary_operators()[20].is_assignment = 1;
	get_binary_operators()[21].token = ar_op_minus_eq;
	get_binary_operators()[21].prec = 100 - 14;
	get_binary_operators()[21].ass = ass_right;
	get_binary_operators()[21].proc = arith_minus_eq;
	get_binary_operators()[21].is_assignment = 1;
	get_binary_operators()[22].token = ar_op_star_eq;
	get_binary_operators()[22].prec = 100 - 14;
	get_binary_operators()[22].ass = ass_right;
	get_binary_operators()[22].proc = arith_multiple_eq;
	get_binary_operators()[22].is_assignment = 1;
	get_binary_operators()[23].token = ar_op_slash_eq;
	get_binary_operators()[23].prec = 100 - 14;
	get_binary_operators()[23].ass = ass_right;
	get_binary_operators()[23].proc = arith_divide_eq;
	get_binary_operators()[23].is_assignment = 1;
	get_binary_operators()[24].token = ar_op_perc_eq;
	get_binary_operators()[24].prec = 100 - 14;
	get_binary_operators()[24].ass = ass_right;
	get_binary_operators()[24].proc = arith_modulo_eq;
	get_binary_operators()[24].is_assignment = 1;
}

void
	arith_binary_init6(void)
{
	get_binary_operators()[25].token = ar_op_less_less_eq;
	get_binary_operators()[25].prec = 100 - 14;
	get_binary_operators()[25].ass = ass_right;
	get_binary_operators()[25].proc = arith_shift_left_eq;
	get_binary_operators()[25].is_assignment = 1;
	get_binary_operators()[26].token = ar_op_great_great_eq;
	get_binary_operators()[26].prec = 100 - 14;
	get_binary_operators()[26].ass = ass_right;
	get_binary_operators()[26].proc = arith_shift_right_eq;
	get_binary_operators()[26].is_assignment = 1;
	get_binary_operators()[27].token = ar_op_amp_eq;
	get_binary_operators()[27].prec = 100 - 14;
	get_binary_operators()[27].ass = ass_right;
	get_binary_operators()[27].proc = arith_bitwise_and_eq;
	get_binary_operators()[27].is_assignment = 1;
	get_binary_operators()[28].token = ar_op_caret_eq;
	get_binary_operators()[28].prec = 100 - 14;
	get_binary_operators()[28].ass = ass_right;
	get_binary_operators()[28].proc = arith_bitwise_xor_eq;
	get_binary_operators()[28].is_assignment = 1;
	get_binary_operators()[29].token = ar_op_pipe_eq;
	get_binary_operators()[29].prec = 100 - 14;
	get_binary_operators()[29].ass = ass_right;
	get_binary_operators()[29].proc = arith_bitwise_or_eq;
	get_binary_operators()[29].is_assignment = 1;
}
