/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer_op.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/07 15:09:37 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/07 15:28:36 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token_id
	tk_op2(t_tokenizer *tk, char ch, t_token_id one, t_token_id two)
{
	if (tk_assume(tk, ch))
		return (two);
	return (one);
}

static t_token_id
	tk_opl(t_tokenizer *tk)
{
	if (tk_assume(tk, '<'))
		return (tk_op2(tk, '-', op_dless, op_dlessdash));
	if (tk_assume(tk, '&'))
		return (op_lessand);
	if (tk_assume(tk, '>'))
		return (op_lessgreat);
	return (op_less);
}

static t_token_id
	tk_opg(t_tokenizer *tk)
{
	if (tk_assume(tk, '>'))
		return (op_dgreat);
	if (tk_assume(tk, '&'))
		return (op_greatand);
	if (tk_assume(tk, '|'))
		return (op_clobber);
	return (op_great);
}

t_token_id
	tk_op(t_tokenizer *tk)
{
	if (tk_assume(tk, '('))
		return (op_lparen);
	if (tk_assume(tk, ')'))
		return (op_rparen);
	if (tk_assume(tk, '&'))
		return (tk_op2(tk, '&', op_and, op_and_if));
	if (tk_assume(tk, '|'))
		return (tk_op2(tk, '|', op_pipe, op_or_if));
	if (tk_assume(tk, ';'))
		return (tk_op2(tk, ';', op_semi, op_dsemi));
	if (tk_assume(tk, '<'))
		return (tk_opl(tk));
	if (tk_assume(tk, '>'))
		return (tk_opg(tk));
	return (token_id_null);
}
