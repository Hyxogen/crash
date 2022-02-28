/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   operator.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 16:02:08 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/28 16:02:09 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static int
	lex_op1(t_lexer *lex, int c)
{
	if (lex->src->cur == c && !lex_quoted(lex))
	{
		lex_update(lex, 0);
		return (1);
	}
	return (0);
}

static t_token_id
	lex_op2(t_lexer *lex, int c, t_token_id one, t_token_id two)
{
	if (lex_op1(lex, c))
		return (two);
	return (one);
}

static t_token_id
	lex_opl(t_lexer *lex)
{
	if (lex_op1(lex, '<'))
		return (lex_op2(lex, '-', op_dless, op_dlessdash));
	if (lex_op1(lex, '&'))
		return (op_lessand);
	if (lex_op1(lex, '>'))
		return (op_lessgreat);
	return (op_less);
}

static t_token_id
	lex_opg(t_lexer *lex)
{
	if (lex_op1(lex, '>'))
		return (op_dgreat);
	if (lex_op1(lex, '&'))
		return (op_greatand);
	if (lex_op1(lex, '|'))
		return (op_clobber);
	return (op_great);
}

t_token_id
	lex_op(t_lexer *lex)
{
	if (lex_op1(lex, '('))
		return (op_lparen);
	if (lex_op1(lex, ')'))
		return (op_rparen);
	if (lex_op1(lex, '&'))
		return (lex_op2(lex, '&', op_and, op_andif));
	if (lex_op1(lex, '|'))
		return (lex_op2(lex, '|', op_pipe, op_orif));
	if (lex_op1(lex, ';'))
		return (lex_op2(lex, ';', op_semi, op_dsemi));
	if (lex_op1(lex, '<'))
		return (lex_opl(lex));
	if (lex_op1(lex, '>'))
		return (lex_opg(lex));
	return (tk_null);
}
