/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   expand_arith_lex.c                               -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:59:36 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:59:36 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"

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
