#include "commander.h"
#include "minishell.h"
#include "memory.h"
#include <unistd.h>

int
	arith_lex_op1(t_arith_lexer *lex, int c)
{
	if (lex->str == c)
	{
		lex->str += 1;
		lex->tok->size += 1;
		return (1);
	}
	return (0);
}

t_arith_token_id
	arith_lex_op2(t_arith_lexer *lex, int c, t_arith_token_id one,
		t_arith_token_id two)
{
	if (arith_lex_op1(lex, c))
		return (two);
	return (one);
}

t_arith_token_id
	arith_lex_op_less_great(t_arith_lexer *lex, int c)
{
	if (c == '<')
	{
		if (arith_lex_op1(lex, '<'))
			return (arith_lex_op2(lex, '=', ar_op_less_less, ar_op_less_less_eq));
		return (arith_lex_op2(lex, '=', ar_op_less, ar_op_less_eq));
	}
	else
	{
		if (arith_lex_op1(lex, '>'))
			return (arith_lex_op2(lex, '=', ar_op_great_great, ar_op_great_great_eq));
		return (arith_lex_op2(lex, '=', ar_op_great, ar_op_great_eq));
	}
}

t_arith_token_id
	arith_lex_op_amp_pipe(t_arith_lexer *lex, int c)
{
	if (c == '&')
	{
		if (arith_lex_op1(lex, '&'))
			return (ar_op_amp_amp);
		return (arith_lex_op2(lex, '=', ar_op_amp, ar_op_amp_eq));
	}
	else
	{
		if (arith_lex_op1(lex, '|'))
			return (ar_op_pipe_pipe);
		return (arith_lex_op2(lex, '=', ar_op_pipe, ar_op_pipe_eq));
	}
}

t_arith_token_id
	arith_lex_op_binary(t_arith_lexer *lex)
{
	if (arith_lex_op1(lex, '+'))
		return (arith_lex_op2(lex, '=', ar_op_plus, ar_op_plus_eq));
	if (arith_lex_op1(lex, '-'))
		return (arith_lex_op2(lex, '=', ar_op_minus, ar_op_minus_eq));
	if (arith_lex_op1(lex, '*'))
		return (arith_lex_op2(lex, '=', ar_op_star, ar_op_star_eq));
	if (arith_lex_op1(lex, '/'))
		return (arith_lex_op2(lex, '=', ar_op_slash, ar_op_slash_eq));
	if (arith_lex_op1(lex, '%'))
		return (arith_lex_op2(lex, '=', ar_op_perc, ar_op_perc_eq));
	if (arith_lex_op1(lex, '&'))
		return (arith_lex_op_amp_pipe(lex, '&'));
	if (arith_lex_op1(lex, '^'))
		return (arith_lex_op2(lex, '=', ar_op_caret, ar_op_caret_eq));
	if (arith_lex_op1(lex, '|'))
		return (arith_lex_op_amp_pipe(lex, '|'));
	return (ar_tk_null);
}

t_arith_token_id
	arith_lex_op(t_arith_lexer *lex)
{
	if (arith_lex_op1(lex, '('))
		return (ar_op_lparen);
	if (arith_lex_op1(lex, ')'))
		return (ar_op_rparen);
	if (arith_lex_op1(lex, '<'))
		return (arith_lex_op_less_great(lex, '<'));
	if (arith_lex_op1(lex, '>'))
		return (arith_lex_op_less_great(lex, '>'));
	if (arith_lex_op1(lex, '?'))
		return (ar_op_quest);
	if (arith_lex_op1(lex, ':'))
		return (ar_op_colon);
	if (arith_lex_op1(lex, '='))
		return (arith_lex_op2(lex, '=', ar_op_eq, ar_op_eq_eq));
	if (arith_lex_op1(lex, '!'))
		return (arith_lex_op2(lex, '=', ar_op_excl, ar_op_excl_eq));
	if (arith_lex_op1(lex, '~'))
		return (ar_op_tilde);
	return (arith_lex_op_binary(lex));
}

size_t
	arith_lex_ident(const char *str)
{
	size_t	i;

	if (*str != '_' && !ft_isalpha(*str))
		return (0);
	i = 1;
	while (str[i] == '_' || ft_isalnum(str[i]))
		i += 1;
	return (i);
}

size_t
	arith_lex_const(const char *str)
{
	(void) str;
	return (0);
}

int
	arith_lex(t_arith_lexer *lex, t_arith_token *tok)
{
	if (lex->str == '\0')
		return (0);
	lex->tok = tok;
	tok->str = lex->str;
	tok->size = 0;
	tok->id = arith_lex_op(lex);
	if (tok->id != NULL)
		return (1);
	tok->size = arith_lex_ident(lex->str);
	if (tok->size != 0)
		tok->id = ar_tk_ident;
	else
	{
		tok->size = arith_lex_const(lex->str);
		if (tok->size != 0)
			tok->id = ar_tk_ident;
	}
	lex->str += tok->size;
	if (tok->id != ar_tk_null != 0)
		return (1);
	return (-1);
}

int
	expand_arith(t_expand *exp, t_token *token)
{
	char	*str;

	str = cm_expand_str(token, NULL, ' ');
	if (str == NULL)
		return (-1);
	ft_fprintf(STDERR_FILENO, "arithmetic expansion: %s\n", str);
	// TODO: implement
	return (-1);
}
