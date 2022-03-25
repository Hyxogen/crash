/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   escape.c                                         -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:22:42 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:22:42 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int
	lex_quoted(t_lexer *lex)
{
	if (lex->src->bslash)
		return (1);
	if (lex->btick)
	{
		if (lex->src->cur != '\\' && lex->src->cur != '`')
			return (1);
		return (0);
	}
	if (lex->quote == 1 && lex->src->cur != '\'')
		return (1);
	if (lex->quote == 2)
	{
		if (lex->src->cur == '$')
			return (0);
		if (lex->src->cur == '`')
			return (0);
		if (lex->src->cur == '\\')
			return (0);
		if (lex->src->cur == '"')
			return (0);
		return (1);
	}
	return (0);
}

int
	lex_bquoted(t_lexer *lex)
{
	if (lex->quote || lex->btick || lex->end != NULL)
	{
		if (lex->src->cur == '$')
			return (1);
		if (lex->src->cur == '`')
			return (1);
		if (lex->src->cur == '"' && !lex->btick && lex->end == NULL)
			return (1);
		if (lex->src->cur == '\\')
			return (1);
		return (0);
	}
	return (1);
}

int
	lex_quoted_int(int quote, int cur)
{
	if (quote == 1 && cur != '\'')
		return (1);
	if (quote == 2)
	{
		if (cur == '$')
			return (0);
		if (cur == '`')
			return (0);
		if (cur == '\\')
			return (0);
		if (cur == '"')
			return (0);
		return (1);
	}
	return (0);
}

int
	lex_bquoted_int(int quote, int cur)
{
	if (quote)
	{
		if (cur == '$')
			return (1);
		if (cur == '`')
			return (1);
		if (cur == '"')
			return (1);
		if (cur == '\\')
			return (1);
		return (0);
	}
	return (1);
}

int
	lex_unquote(char *str)
{
	char	*dst;
	int		quote;

	dst = str;
	quote = 0;
	while (*str != '\\' && *str != '"' && *str != '\'' && *str != '\0')
		str += 1;
	if (*str == '\0')
		return (0);
	while (str[0] != '\0')
	{
		if (lex_quoted_int(quote, str[0]))
			*dst++ = *str;
		else if (str[0] == '\\' && lex_bquoted_int(quote, str[1]))
			*dst++ = *++str;
		else if (str[0] == '"')
			quote = 2 - quote;
		else if (str[0] == '\'')
			quote = 1 - quote;
		else
			*dst++ = *str;
		str += 1;
	}
	*dst = '\0';
	return (1);
}
