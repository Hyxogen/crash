#include "commander.h"

#include "minishell.h"

int
	cm_and_if(const t_snode *node, const int io[3])
{
	int	return_code;

	sh_assert(node->childs_size >= 2);
	return_code = commandeer(node->childs[0], io);
	if (return_code == 0)
		return (commandeer(node->childs[1], io));
	return (return_code);
}

int
	cm_or_if(const t_snode *node, const int io[3])
{
	int	return_code;

	sh_assert(node->childs_size >= 2);
	return_code = commandeer(node->childs[0], io);
	if (return_code != 0)
		return (commandeer(node->childs[1], io));
	return (return_code);
}
