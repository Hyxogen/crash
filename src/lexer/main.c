#include "lexer.h"

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
		end = src_check_end(lex->src, lex->end, lex->here_flags);
		printf("end check: %d\n", (lex->src->cur == -1 && end));
		return (lex->src->cur == -1 && end);
	}
	end = ft_strchr(sep, lex->src->cur) != NULL && lex->src->cur != '\0';
	return ((end && !lex_quoted(lex)) || lex->src->cur == -1);
}

void
	lex_main(t_lexer *lex)
{
	/* TODO: error handling */
	while (!lex_check_end(lex))
	{
		if (lex->src->cur == -1)
			return ;
		if (lex_quoted(lex) || !lex_special(lex))
			lex_update(lex, 1);
	}
}