#include "input.h"
#include <libft.h>
#include <stdlib.h>
#include <readline/readline.h>

char
	*input_readline(t_readline *rl, const char *prompt)
{
	(void) rl;
	return (readline(prompt));
}

int	
	input_readchar(t_input *in)
{
	int	c;

	if (in->line == NULL && in->more)
		in->line = input_readline(&in->rl, "> ");
	else if (in->line == NULL)
		in->line = input_readline(&in->rl, "$ ");
	if (in->line == NULL)
		return (-1);
	c = (unsigned char) in->line[in->index];
	in->index += 1;
	in->more = 1;
	if (c == '\0')
	{
		free(in->line);
		in->line = NULL;
		in->index = 0;
		return ('\n');
	}
	return (c);
}
