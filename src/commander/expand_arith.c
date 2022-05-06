#include "commander.h"
#include "minishell.h"
#include "memory.h"
#include <unistd.h>
#include <stdlib.h>

long			expand_arith_str(const char *str, int *error);
t_arith_value	arith_parse_binary(t_arith_parser *pr, t_arith_value lhs, int min_prec);

long
	identity(const char *ptr, long a, long b, long c)
{
	(void) ptr;
	(void) b;
	(void) c;
	return (a);
}

static const t_arith_operator binary_operators[] = {
	{ ar_op_star, 100 - 3, ass_left, arith_multiply, 0 },
	{ ar_op_slash, 100 - 3, ass_left, arith_divide, 0 },
	{ ar_op_perc, 100 - 3, ass_left, arith_modulo, 0 },
	{ ar_op_plus, 100 - 4, ass_left, arith_plus, 0 },
	{ ar_op_minus, 100 - 4, ass_left, arith_minus, 0 },
	{ ar_op_less_less, 100 - 5, ass_left, arith_shift_left, 0 },
	{ ar_op_great_great, 100 - 5, ass_left, arith_shift_right, 0 },
	{ ar_op_less, 100 - 6, ass_left, arith_less_than, 0 },
	{ ar_op_less_eq, 100 - 6, ass_left, arith_greater_than_or_eq, 0 },
	{ ar_op_great, 100 - 6, ass_left, arith_greater_than, 0 },
	{ ar_op_great_eq, 100 - 6, ass_left, arith_greater_than_or_eq, 0 },
	{ ar_op_eq_eq, 100 - 7, ass_left, arith_equal, 0 },
	{ ar_op_excl_eq, 100 - 7, ass_left, arith_not_equal, 0 },
	{ ar_op_amp, 100 - 8, ass_left, arith_bitwise_and, 0 },
	{ ar_op_caret, 100 - 9, ass_left, arith_bitwise_xor, 0 },
	{ ar_op_pipe, 100 - 10, ass_left, arith_bitwise_or, 0 },
	{ ar_op_amp_amp, 100 - 11, ass_left, arith_logical_and, 0 },
	{ ar_op_pipe_pipe, 100 - 12, ass_left, arith_logical_or, 0 },
	{ ar_op_quest, 100 - 13, ass_right, arith_ternary, 0 },
	{ ar_op_eq, 100 - 14, ass_right, arith_assign, 1 },
	{ ar_op_plus_eq, 100 - 14, ass_right, arith_plus_eq, 1 },
	{ ar_op_minus_eq, 100 - 14, ass_right, arith_minus_eq, 1 },
	{ ar_op_star_eq, 100 - 14, ass_right, arith_multiple_eq, 1 },
	{ ar_op_slash_eq, 100 - 14, ass_right, arith_divide_eq, 1 },
	{ ar_op_perc_eq, 100 - 14, ass_right, arith_modulo_eq, 1 },
	{ ar_op_less_less_eq, 100 - 14, ass_right, arith_shift_left_eq, 1 },
	{ ar_op_great_great_eq, 100 - 14, ass_right, arith_shift_right_eq, 1 },
	{ ar_op_amp_eq, 100 - 14, ass_right, arith_bitwise_and_eq, 1 },
	{ ar_op_caret_eq, 100 - 14, ass_right, arith_bitwise_xor_eq, 1 },
	{ ar_op_pipe_eq, 100 - 14, ass_right, arith_bitwise_or_eq, 1 },
};

static const t_arith_operator unary_operators[] = {
	{ ar_op_plus, 100 - 2, ass_right, arith_identity, 0 },
	{ ar_op_minus, 100 - 2, ass_right, arith_negate, 0 },
	{ ar_op_excl, 100 - 2, ass_right, arith_logical_not, 0 },
	{ ar_op_tilde, 100 - 2, ass_right, arith_bitwise_not, 0 },
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
		tok->id = ar_tk_const * (tok->size != 0) + tok->id * (tok->size == 0);
	}
	lex->str += tok->size;
	return ((tok->id != ar_tk_null) * 2 - 1);
}

const t_arith_operator
	*arith_get_unary_op(t_arith_token_id id)
{
	size_t	i;

	i = 0;
	while (i < sizeof(unary_operators) / sizeof(*unary_operators))
	{
		if (unary_operators[i].token == id)
			return (&unary_operators[i]);
		i += 1;
	}
	return (NULL);
}

const t_arith_operator
	*arith_get_binary_op(t_arith_token_id id)
{
	size_t	i;

	i = 0;
	while (i < sizeof(binary_operators) / sizeof(*binary_operators))
	{
		if (binary_operators[i].token == id)
			return (&binary_operators[i]);
		i += 1;
	}
	return (NULL);
}

t_arith_value
	arith_parse_unary(t_arith_parser *pr)
{
	const t_arith_operator	*op;
	t_arith_value			expr;
	const char				*tmp;
	const char				*key;

	op = arith_get_unary_op(pr->next.id);
	expr.value = 0;
	expr.str = NULL;
	expr.size = 0;
	if (op != NULL)
	{
		arith_lex(pr->lex, &pr->next);
		expr = arith_parse_unary(pr);
		if (!pr->is_fake || pr->error)
		{
			expr.value = op->proc(NULL, expr.value, 0, 0);
		}
		return (expr);
	}
	if (pr->next.id == ar_op_lparen)
	{
		arith_lex(pr->lex, &pr->next);
		expr = arith_parse_unary(pr);
		expr = arith_parse_binary(pr, expr, 0);
		if (pr->next.id == ar_op_rparen)
			arith_lex(pr->lex, &pr->next);
		else
			pr->error = 1;
		return (expr);
	}
	if (pr->next.id == ar_tk_const)
	{
		key = ft_strndup(pr->next.str, pr->next.size);
		if (sh_arith_atol(key, &expr.value) < 0)
			pr->error = 1;
		free((void*) key);
		arith_lex(pr->lex, &pr->next);
		return (expr);
	}
	if (pr->next.id == ar_tk_ident)
	{
		key = ft_strndup(pr->next.str, pr->next.size);
		tmp = sh_getenv(key, "0");
		free((void*) key);
		expr.value = expand_arith_str(tmp, &pr->error);
		expr.str = pr->next.str;
		expr.size = pr->next.size;
		arith_lex(pr->lex, &pr->next);
		return (expr);
	}
	pr->error = 1;
	return (expr);
}

/* TODO: check recursion level */
t_arith_value
	arith_parse_binary(t_arith_parser *pr, t_arith_value lhs, int min_prec)
{
	const t_arith_operator	*lookahead;
	const t_arith_operator	*op;
	t_arith_value			rhs;
	t_arith_value			super_rhs;
	const char				*tmp;

	lookahead = arith_get_binary_op(pr->next.id);
	while (lookahead != NULL && lookahead->prec >= min_prec)
	{
		op = lookahead;
		arith_lex(pr->lex, &pr->next);
		if (op->token == ar_op_quest)
		{
			pr->is_fake += !lhs.value;
			rhs = arith_parse_unary(pr);
			rhs = arith_parse_binary(pr, rhs, 0);
			pr->is_fake -= !lhs.value;
			if (pr->next.id == ar_op_colon)
			{
				arith_lex(pr->lex, &pr->next);
				pr->is_fake += !!lhs.value;
				super_rhs = arith_parse_unary(pr);
				super_rhs = arith_parse_binary(pr, super_rhs, 100 - 13);
				pr->is_fake -= !!lhs.value;
				if (!pr->is_fake || pr->error)
					lhs.value = op->proc(NULL, lhs.value, rhs.value, super_rhs.value);
				lookahead = arith_get_binary_op(pr->next.id);
			}
			else
				pr->error = 1;
		}
		else
		{
			if (op->token == ar_op_amp_amp)
			{
				pr->is_fake += !lhs.value;
				rhs = arith_parse_unary(pr);
				rhs = arith_parse_binary(pr, rhs, 100 - 12);
				pr->is_fake -= !lhs.value;
				lookahead = arith_get_binary_op(pr->next.id);
			}
			else if (op->token == ar_op_pipe_pipe)
			{
				pr->is_fake += !!lhs.value;
				rhs = arith_parse_unary(pr);
				rhs = arith_parse_binary(pr, rhs, 100 - 13);
				pr->is_fake -= !!lhs.value;
				lookahead = arith_get_binary_op(pr->next.id);
			}
			else
			{
				rhs = arith_parse_unary(pr);
				lookahead = arith_get_binary_op(pr->next.id);
				while (lookahead != NULL && (lookahead->prec > op->prec ||
					(lookahead->ass == ass_right && lookahead->prec == op->prec)))
				{
					rhs = arith_parse_binary(pr, rhs,
						op->prec + (lookahead->prec > op->prec));
					lookahead = arith_get_binary_op(pr->next.id);
				}
			}
			if (op->is_assignment && lhs.str == NULL)
				pr->error = 1;
			else if (!pr->is_fake || pr->error)
			{
				tmp = ft_strndup(lhs.str, lhs.size);
				lhs.value = op->proc(tmp, lhs.value, rhs.value, 0);
				free((void*) tmp);
			}
		}
		lhs.str = NULL;
		lhs.size = 0;
	}
	return (lhs);
}

long
	expand_arith_str(const char *str, int *error)
{
	t_arith_lexer	lex;
	t_arith_parser	pr;
	t_arith_value	result;

	lex.str = str;
	pr.lex = &lex;
	pr.error = 0;
	pr.is_fake = 0;
	arith_lex(pr.lex, &pr.next);
	result = arith_parse_unary(&pr);
	result = arith_parse_binary(&pr, result, 0);
	if (pr.next.id != ar_tk_eof || pr.error)
		*error = 1;
	return (result.value);
}

int
	expand_arith(t_expand *exp, t_token *token)
{
	char	*str;
	long	result;
	int		error;

	str = cm_expand_str(token, NULL, ' ');
	if (str == NULL)
		return (-1);
	error = 0;
	result = expand_arith_str(str, &error);
	free(str);
	if (error)
		return (-1);
	str = sh_safe_malloc(23);
	sh_ltoa(result, str, 23);
	expansion_add_part(exp, sh_strlst_new(str), 0);
	return (0);
}
