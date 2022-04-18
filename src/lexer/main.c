/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   main.c                                           -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:22:48 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:22:48 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <unistd.h>

static int
	lex_check_end(t_lexer *lex)
{
	static const char	*sep = "\n\t\r\v\f &();<>|";
	int					end;

	if (lex->id == lx_parameter)
		return (lex->src->cur == '}'
			&& !lex_quoted(lex));
	if (lex->id == lx_arithmetic)
		return (lex->src->cur == ')' && lex->src->nex == ')'
			&& !lex_quoted(lex) && lex->depth == 0);
	if (lex->end != NULL)
	{
		end = src_check_end(lex, lex->end, lex->here_flags);
		return (lex->src->cur == -1 || end);
	}
	end = (ft_strchr(sep, lex->src->cur) != NULL && lex->src->cur != '\0');
	// if (lex->id == lx_command)
	// 	return ((end && !lex_quoted(lex)) && lex->src->cur != -1);
	return ((end && !lex_quoted(lex)) || lex->src->cur == -1);
}

int
	lex_main(t_lexer *lex)
{
	while (!lex_check_end(lex))
	{
		if (lex->src->cur == -1)
			return (-1);
		if (lex->id == lx_arithmetic)
		{
			if (lex->src->cur == '(')
				lex->depth += 1;
			if (lex->src->cur == ')')
				lex->depth -= 1;
			if (lex->depth < 0)
				return (-1);
		}
		if (lex_quoted(lex) || !lex_special(lex))
		{
			if (!lex->quote && lex->src->bslash)
				lex->new_part = 1;
			lex_update(lex, 0);
		}
	}
	return (1);
}
