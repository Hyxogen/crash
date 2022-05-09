#include "commander.h"

#include <unistd.h>

int
	_cm_redi_get_from_index(const t_snode *redi_node)
{
	int	from_fd;

	from_fd = -1;
	if (redi_node->token.id == tk_ionumber)
	{
		from_fd = ft_atol(redi_node->token.str);
		if (from_fd < 0 || from_fd >= INT_MAX)
		{
			sh_err1("invalid file descriptor");
			return (-2);
		}
	}
	if (from_fd == -1)
		return (-1);
	if (from_fd == STDIN_FILENO)
		return (SH_STDIN_INDEX);
	if (from_fd == STDOUT_FILENO)
		return (SH_STDOUT_INDEX);
	if (from_fd == STDERR_FILENO)
		return (SH_STDERR_INDEX);
	return (3);
}

static int
	_cm_handle_builtin_redi_node(const t_snode *redi_node, int io[3])
{
	char	**filen;
	int		return_code;

	if (redi_node->childs_size == 0)
		return (sh_err1("no file specified"), -1);
	sh_assert(redi_node->childs[0]->token.id != tk_invalid);
	filen = cm_wildcard_expand(&redi_node->childs[0]->token);
	if (!filen)
		return (-1);
	if (!*filen || *(filen + 1))
	{
		sh_strlst_clear(filen);
		return (sh_err1("ambigious redirect"), -1);
	}
	return_code = _cm_handle_builtin_redi_node_noerr(redi_node, *filen, io);
	sh_strlst_clear(filen);
	return (return_code);
}

int
	_cm_setup_builtin_redirects(const t_snode *redi_list, int io[3])
{
	t_snode	*node;
	size_t	size;
	size_t	index;
	int		rc;

	sh_assert(redi_list->type == sx_io_redirect_list);
	index = 0;
	size = redi_list->childs_size;
	while (index < size)
	{
		node = redi_list->childs[index];
		rc = _cm_handle_builtin_redi_node(node, io);
		if (rc)
			return (rc);
		index++;
	}
	return (0);
}
