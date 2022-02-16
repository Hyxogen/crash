#include "lexer.h"
#include <libft.h>

int
	lexer_ioend(t_lexer *lex)
{
	size_t	index;

	index = 0;
	if (lex->cur != -1)
	{
		if (lex->cur != lex->end[0])
			return (0);
		index += 1;
	}
	if (lex->next != -1)
	{
		if (lex->next != lex->end[1])
			return (0);
		if (lex->next == '\n' && lex->end[1] == '\0')
			return (1);
		index += 1;
	}
	if (lex->in->line == NULL && lex->in->more)
		lex->in->line = input_readline(&lex->in->rl, "> ");
	else if (lex->in->line == NULL)
		lex->in->line = input_readline(&lex->in->rl, "$ ");
	if (lex->in->line == NULL || lex->in->index != index)
		return (0);
	return (ft_memcmp(lex->in->line, lex->end, ft_strlen(lex->end) + 1) == 0);
}
