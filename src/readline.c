#include "minishell.h"
#include <readline/readline.h>

char
	*sh_readline(t_readline *rl, const char *prompt)
{
	(void) rl;
	return (readline(prompt));
}
