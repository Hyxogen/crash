#include "commander.h"
#include "minishell.h"
#include "memory.h"
#include <unistd.h>
#include <stdlib.h>

long			expand_arith_str(const char *str, int *error, int recursion_level);
t_arith_value	arith_parse_binary(t_arith_parser *pr, t_arith_value lhs, int min_prec);

long
	identity(const char *ptr, long a, long b, long c)
{
	(void) ptr;
	(void) b;
	(void) c;
	return (a);
}

void
	arith_init(void)
{
	arith_binary_init();
	arith_optok_init();
}

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
	while (i < SH_OP_TOK_COUNT)
	{
		tmp = ft_strlen(get_operator_tokens()[i].str);
		if (ft_memcmp(get_operator_tokens()[i].str, lex->str, tmp) == 0)
		{
			if (tmp >= len)
			{
				len = tmp;
				result = get_operator_tokens()[i].id;
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

t_arith_op
	*arith_get_unary_op(t_arith_token_id id)
{
	size_t	i;

	i = 0;
	while (i < SH_UNARY_OP_COUNT)
	{
		if (get_unary_operators()[i].token == id)
			return (&get_unary_operators()[i]);
		i += 1;
	}
	return (NULL);
}

const t_arith_op
	*arith_get_binary_op(t_arith_token_id id)
{
	size_t	i;

	i = 0;
	while (i < SH_BINARY_OP_COUNT)
	{
		if (get_binary_operators()[i].token == id)
			return (&get_binary_operators()[i]);
		i += 1;
	}
	return (NULL);
}

t_arith_value
	arith_parse_binary(t_arith_parser *pr, t_arith_value lhs, int min_prec)
{
	const t_arith_op	*lookahead;
	const t_arith_op	*op;
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
	expand_arith_str(const char *str, int *error, int recursion_level)
{
	t_arith_lexer	lex;
	t_arith_parser	pr;
	t_arith_value	result;

	if (recursion_level >= 1000)
	{
		sh_err1("operation already in progress");
		*error = 1;
		return (0);
	}
	lex.str = str;
	pr.lex = &lex;
	pr.error = 0;
	pr.is_fake = 0;
	pr.recursion_level = recursion_level;
	arith_lex(pr.lex, &pr.next);
	result = arith_parse_unary(&pr);
	result = arith_parse_binary(&pr, result, 0);
	if (pr.next.id != ar_tk_eof || pr.error)
		*error = 1;
	return (result.value);
}

/* ODOT: this is not proper error handling */
int
	expand_arith(t_expand *exp, t_token *token)
{
	char	*str;
	long	result;
	int		error;

	str = cm_expand_str(token, NULL, ' ', 0);
	if (str == NULL)
		return (-1);
	error = 0;
	result = expand_arith_str(str, &error, 0);
	free(str);
	if (error)
	{
		sh_err1("invalid arithmetic expansion");
		return (-1);
	}
	str = sh_safe_malloc(23);
	sh_ltoa(result, str, 23);
	expansion_add_part(exp, sh_strlst_new(str), 0);
	return (0);
}
