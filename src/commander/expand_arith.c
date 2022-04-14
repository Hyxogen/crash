#include "commander.h"
#include "minishell.h"
#include "memory.h"
#include <unistd.h>

int
	expand_arith(t_expand *exp, t_token *token)
{
	char	*str;

	str = cm_expand_str(token, NULL, ' ');
	if (str == NULL)
		return (-1);
	ft_fprintf(STDERR_FILENO, "arithmetic expansion: %s\n", str);
	// TODO: implement
	return (-1);
}
