#include "commander.h"
#include "minishell.h"

#include <libft.h>
#include <stdlib.h>


#include <stdio.h>

pid_t
	cm_function_define(const t_snode *node, const int io[3])
{
	(void) io;
	sh_assert(node->childs_size >= 1);
	sh_add_function(node->token.str, node->childs[0]);
	return (cm_convert_retcode(0));
}

/* TODO perhaps try to change implementation so that the
	pipe sequence won't have to wait for the entire function to finish? */
pid_t
	cm_function(const t_snode *node, const int io[3])
{
	int		func_io[3];

	sh_assert(node != NULL);
	sh_assert(node->childs_size != 0);
	ft_memcpy(func_io, io, sizeof(func_io));
	_cm_setup_builtin_redirects(node->childs[node->childs_size - 1], func_io);
	return (cm_convert_retcode(commandeer(node->childs[0], func_io)));
}
