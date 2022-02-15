#include "lexer.h"

static int
	lexer_op1(t_lexer *lex, int c)
{
	if (lex->cur == c && !lexer_quoted(lex))
	{
		lexer_read(lex, 0);
		return (1);
	}
	return (0);
}

static t_token_id
	lexer_op2(t_lexer *lex, int c, t_token_id one, t_token_id two)
{
	if (lexer_op1(lex, c))
		return (two);
	return (one);
}

static t_token_id
	lexer_opl(t_lexer *lex)
{
	if (tk_assume(lex, '<'))
		return (tk_op2(lex, '-', op_dless, op_dlessdash));
	if (tk_assume(lex, '&'))
		return (op_lessand);
	if (tk_assume(lex, '>'))
		return (op_lessgreat);
	return (op_less);
}

static t_token_id
	lexer_opg(t_lexer *lex)
{
	if (tk_assume(lex, '>'))
		return (op_dgreat);
	if (tk_assume(lex, '&'))
		return (op_greatand);
	if (tk_assume(lex, '|'))
		return (op_clobber);
	return (op_great);
}

t_token_id
	lexer_op(t_lexer *lex)
{
	if (tk_assume(lex, '('))
		return (op_lparen);
	if (tk_assume(lex, ')'))
		return (op_rparen);
	if (tk_assume(lex, '&'))
		return (tk_op2(lex, '&', op_and, op_andif));
	if (tk_assume(lex, '|'))
		return (tk_op2(lex, '|', op_pipe, op_orif));
	if (tk_assume(lex, ';'))
		return (tk_op2(lex, ';', op_semi, op_dsemi));
	if (tk_assume(lex, '<'))
		return (tk_opl(lex));
	if (tk_assume(lex, '>'))
		return (tk_opg(lex));
	return (tk_null);
}