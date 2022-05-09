#include "commander.h"
#include "minishell.h"
#include "memory.h"
#include <unistd.h>
#include <stdlib.h>

long			expand_arith_str(const char *str,
					int *error, int recursion_level);
t_arith_value	arith_parse_binary(t_arith_parser *pr,
					t_arith_value lhs, int min_prec);

void
	arith_init(void)
{
	arith_binary_init();
	arith_optok_init();
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

t_arith_op
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
