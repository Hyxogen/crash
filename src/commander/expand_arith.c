#include "commander.h"
#include "minishell.h"
#include "memory.h"
#include <unistd.h>
#include <stdlib.h>

long
	identity(const char *ptr, long a, long b, long c)
{
	(void) ptr;
	(void) b;
	(void) c;
	return (a);
}

static const t_arith_operator binary_operators[] = {
	{ ar_op_star, 100 - 3, ass_left, identity },
	{ ar_op_slash, 100 - 3, ass_left, identity },
	{ ar_op_perc, 100 - 3, ass_left, identity },
	{ ar_op_plus, 100 - 4, ass_left, arith_plus },
	{ ar_op_minus, 100 - 4, ass_left, identity },
	{ ar_op_less_less, 100 - 5, ass_left, identity },
	{ ar_op_great_great, 100 - 5, ass_left, identity },
	{ ar_op_less, 100 - 6, ass_left, identity },
	{ ar_op_less_eq, 100 - 6, ass_left, identity },
	{ ar_op_great, 100 - 6, ass_left, identity },
	{ ar_op_great_eq, 100 - 6, ass_left, identity },
	{ ar_op_eq_eq, 100 - 7, ass_left, identity },
	{ ar_op_excl_eq, 100 - 7, ass_left, identity },
	{ ar_op_amp, 100 - 8, ass_left, identity },
	{ ar_op_caret, 100 - 9, ass_left, identity },
	{ ar_op_pipe, 100 - 10, ass_left, identity },
	{ ar_op_amp_amp, 100 - 11, ass_left, identity },
	{ ar_op_pipe_pipe, 100 - 12, ass_left, identity },
	{ ar_op_quest, 100 - 13, ass_right, identity },
	{ ar_op_eq, 100 - 14, ass_right, identity },
	{ ar_op_plus_eq, 100 - 14, ass_right, identity },
	{ ar_op_minus_eq, 100 - 14, ass_right, identity },
	{ ar_op_star_eq, 100 - 14, ass_right, identity },
	{ ar_op_slash_eq, 100 - 14, ass_right, identity },
	{ ar_op_perc_eq, 100 - 14, ass_right, identity },
	{ ar_op_less_less_eq, 100 - 14, ass_right, identity },
	{ ar_op_great_great_eq, 100 - 14, ass_right, identity },
	{ ar_op_amp_eq, 100 - 14, ass_right, identity },
	{ ar_op_caret_eq, 100 - 14, ass_right, identity },
	{ ar_op_pipe_eq, 100 - 14, ass_right, identity },
};

static const t_arith_operator unary_operators[] = {
	{ ar_op_plus, 100 - 2, ass_right, identity },
	{ ar_op_minus, 100 - 2, ass_right, identity },
	{ ar_op_excl, 100 - 2, ass_right, identity },
	{ ar_op_tilde, 100 - 2, ass_right, identity },
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

	op = arith_get_unary_op(pr->next.id);
	if (op != NULL)
	{
		arith_lex(pr->lex, &pr->next);
		expr = arith_parse_unary(pr);
		expr.value = op->proc(NULL, expr.value, 0, 0);
		expr.str = NULL;
		expr.size = 0;
		return (expr);
	}
	if (pr->next.id == ar_op_lparen)
	{
		arith_lex(pr->lex, &pr->next);
		expr = arith_parse_unary(pr);
		if (pr->next.id == ar_op_rparen)
			arith_lex(pr->lex, &pr->next);
		else
			pr->error = 1;
		return (expr);
	}
	if (pr->next.id == ar_tk_const)
	{
		/* TODO: check overflow and invalid integer */
		tmp = ft_strndup(pr->next.str, pr->next.size);
		sh_atol(tmp, &expr.value);
		expr.str = NULL;
		expr.size = 0;
		free(tmp);
		arith_lex(pr->lex, &pr->next);
		return (expr);
	}
	if (pr->next.id == ar_tk_ident)
	{
		/* TODO: get env value */
		expr.str = pr->next.str;
		expr.size = pr->next.size;
		arith_lex(pr->lex, &pr->next);
		return (expr);
	}
	pr->error = 1;
	return (expr);
}

/* TODO: ternary operator */
t_arith_value
	arith_parse_binary(t_arith_parser *pr, t_arith_value lhs, int min_prec)
{
	const t_arith_operator	*lookahead;
	const t_arith_operator	*op;
	t_arith_value			rhs;
	const char				*tmp;

	lookahead = arith_get_binary_op(pr->next.id);
	while (lookahead != NULL && lookahead->prec >= min_prec)
	{
		op = lookahead;
		arith_lex(pr->lex, &pr->next);
		rhs = arith_parse_unary(pr);
		lookahead = arith_get_binary_op(pr->next.id);
		while (lookahead != NULL && (lookahead->prec > op->prec ||
			(lookahead->ass == ass_right && lookahead->prec == op->prec)))
		{
			rhs = arith_parse_binary(pr, rhs,
				op->prec + (lookahead->prec > op->prec));
			lookahead = arith_get_binary_op(pr->next.id);
		}
		tmp = ft_strndup(lhs.str, lhs.size);
		lhs.value = op->proc(tmp, lhs.value, rhs.value, 0);
		lhs.str = NULL;
		lhs.size = 0;
		free(tmp);
	}
	return (lhs);
}

int
	expand_arith(t_expand *exp, t_token *token)
{
	char			*str;
	t_arith_lexer	lex;
	t_arith_token	tok;
	t_arith_parser	pr;
	t_arith_value	result;

	lex.str = cm_expand_str(token, NULL, ' ');
	if (lex.str == NULL)
		return (-1);
	ft_fprintf(STDERR_FILENO, "arithmetic expansion: %s\n", lex.str);
	lex.tok = &tok;
	pr.lex = &lex;
	pr.error = 0;
	arith_lex(pr.lex, &pr.next);
	result = arith_parse_unary(&pr);
	result = arith_parse_binary(&pr, result, 0);
	if (pr.next.id != ar_tk_eof || pr.error)
		return (-1);
	/* TODO: ltoa */
	expansion_add_part(exp, sh_strlst_new(ft_itoa(result.value)), 0);
	return (0);
}
