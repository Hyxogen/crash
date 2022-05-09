#include "minishell.h"

/* this is not a global */
t_minishell
	*sh(void)
{
	static t_minishell	shell;

	return (&shell);
}
