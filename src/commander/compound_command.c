#include "commander.h"

#include <stdlib.h>

static pid_t
	cm_compound_list_no_subshell(const t_snode *node,
			const int io[SH_STDIO_SIZE])
{
	size_t	index;
	size_t	count;
	int		return_code;
	int		new_io[SH_STDIO_SIZE];

	return_code = EXIT_SUCCESS;
	index = 0;
	count = node->childs_size;
	ft_memcpy(new_io, io, sizeof(new_io));
	_cm_setup_builtin_redirects(node->childs[node->childs_size - 1], new_io);
	while (index + 1 < count)
	{
		return_code = commandeer(node->childs[index], new_io);
		index++;
	}
	return (return_code_to_internal_pid(return_code));
}

static pid_t
	cm_compound_list_subshell(const t_snode *node, const int io[SH_STDIO_SIZE])
{
	pid_t	subshell_pid;

	subshell_pid = sh_fork();
	if (subshell_pid == 0)
	{
		setup_default_signal_handlers();
		exit(cm_compound_list_no_subshell(node, io));
	}
	return (subshell_pid);
}

pid_t
	cm_compound_list(const t_snode *node, const int io[SH_STDIO_SIZE])
{
	if (node->flags & flag_subshell)
	{
		return (cm_compound_list_subshell(node, io));
	}
	return (cm_compound_list_no_subshell(node, io));
}
