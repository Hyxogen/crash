#include "commander.h"
#include "minishell.h"
#include "memory.h"
#include <unistd.h>

static const t_arith_operator binary_operators[] = {
	{ ar_op_star, 3, ass_left },
	{ ar_op_slash, 3, ass_left },
	{ ar_op_perc, 3, ass_left },
	{ ar_op_plus, 4, ass_left },
	{ ar_op_minus, 4, ass_left },
	{ ar_op_less_less, 5, ass_left },
	{ ar_op_great_great, 5, ass_left },
	{ ar_op_less, 6, ass_left },
	{ ar_op_less_eq, 6, ass_left },
	{ ar_op_great, 6, ass_left },
	{ ar_op_great_eq, 6, ass_left },
	{ ar_op_eq_eq, 7, ass_left },
	{ ar_op_excl_eq, 7, ass_left },
	{ ar_op_amp, 8, ass_left },
	{ ar_op_caret, 9, ass_left },
	{ ar_op_pipe, 10, ass_left },
	{ ar_op_amp_amp, 11, ass_left },
	{ ar_op_pipe_pipe, 12, ass_left },
	{ ar_op_quest, 13, ass_right },
	{ ar_op_eq, 14, ass_right },
	{ ar_op_plus_eq, 14, ass_right },
	{ ar_op_minus_eq, 14, ass_right },
	{ ar_op_star_eq, 14, ass_right },
	{ ar_op_slash_eq, 14, ass_right },
	{ ar_op_perc_eq, 14, ass_right },
	{ ar_op_less_less_eq, 14, ass_right },
	{ ar_op_great_great_eq, 14, ass_right },
	{ ar_op_amp_eq, 14, ass_right },
	{ ar_op_caret_eq, 14, ass_right },
	{ ar_op_pipe_eq, 14, ass_right },
};

static const t_arith_operator unary_operators[] = {
	{ ar_op_plus, 2, ass_right },
	{ ar_op_minus, 2, ass_right },
	{ ar_op_excl, 2, ass_right },
	{ ar_op_tilde, 2, ass_right },
};

static const t_arith_optok operator_tokens[] = {
	{ "<", ar_op_less },
	{ "<=", ar_op_less_eq },
	{ "<<", ar_op_less_less },
	{ "<<=", ar_op_less_less_eq },
	{ ">", ar_op_great },
	{ ">=", ar_op_great_eq },
	{ ">>", ar_op_great_great },
	{ ">>=", ar_op_great_great_eq },
	{ "&", ar_op_amp },
	{ "&&", ar_op_amp_amp },
	{ "&=", ar_op_amp_eq },
	{ "|", ar_op_pipe },
	{ "||", ar_op_pipe_pipe },
	{ "|=", ar_op_pipe_eq },
	{ "+", ar_op_plus },
	{ "+=", ar_op_plus_eq },
	{ "-", ar_op_minus },
	{ "-=", ar_op_minus_eq },
	{ "*", ar_op_star },
	{ "*=", ar_op_star_eq },
	{ "/", ar_op_slash },
	{ "/=", ar_op_slash_eq },
	{ "%", ar_op_perc },
	{ "%=", ar_op_perc_eq },
	{ "^", ar_op_caret },
	{ "^=", ar_op_caret_eq },
	{ "(", ar_op_lparen },
	{ ")", ar_op_rparen },
	{ "?", ar_op_quest },
	{ ":", ar_op_colon },
	{ "=", ar_op_eq },
	{ "==", ar_op_eq_eq },
	{ "!=", ar_op_excl_eq },
	{ "!", ar_op_excl },
	{ "~", ar_op_tilde },
};

t_arith_token_id
	arith_lex_op(t_arith_lexer *lex)
{
	size_t				i;
	size_t				len;
	size_t				tmp;
	t_arith_token_id	result;

	i = 0;
	result = ar_tk_null;
	len = 0;
	while (i < sizeof(operator_tokens) / sizeof(*operator_tokens))
	{
		tmp = ft_strlen(operator_tokens[i].str);
		if (ft_memcmp(operator_tokens[i].str, lex->str, tmp) == 0)
		{
			if (tmp >= len)
			{
				len = tmp;
				result = operator_tokens[i].id;
			}
		}
		i += 1;
	}
	lex->str += len;
	return (result);
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
	size_t	i;

	if (!ft_isdigit(*str))
		return (0);
	i = 1;
	while (str[i] == '_' || ft_isalnum(str[i]))
		i += 1;
	return (i);
}

int
	arith_lex(t_arith_lexer *lex, t_arith_token *tok)
{
	while (ft_isspace(*lex->str))
		lex->str += 1;
	if (*lex->str == '\0')
	{
		tok->id = ar_tk_eof;
		return (0);
	}
	lex->tok = tok;
	tok->str = lex->str;
	tok->size = 0;
	tok->id = arith_lex_op(lex);
	if (tok->id != ar_tk_null)
		return (1);
	tok->size = arith_lex_ident(lex->str);
	if (tok->size != 0)
		tok->id = ar_tk_ident;
	else
	{
		tok->size = arith_lex_const(lex->str);
		if (tok->size != 0)
			tok->id = ar_tk_const;
	}
	lex->str += tok->size;
	if (tok->id != ar_tk_null)
		return (1);
	return (-1);
}

int
	expand_arith(t_expand *exp, t_token *token)
{
	char			*str;
	t_arith_lexer	lex;
	t_arith_token	tok;

	str = cm_expand_str(token, NULL, ' ');
	if (str == NULL)
		return (-1);
	ft_fprintf(STDERR_FILENO, "arithmetic expansion: %s\n", str);
	lex.str = str;
	lex.tok = &tok;
	while (1)
	{
		if (arith_lex(&lex, &tok) != 1)
			break ;
		ft_fprintf(STDERR_FILENO, "arithmetic token: %d\n", tok.id);
	}
	// TODO: implement
	return (-1);
}
