#include "commander.h"
#include "minishell.h"

#include <libft.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

static int
	_cm_open_file(const char *filen, int flags, int mode)
{
	int	result_fd;

	result_fd = sh_open(filen, flags, mode);
	sh_fdctl(result_fd, SH_FD_FIOCLEX, 1);
	return (result_fd);
}

static int
	_cm_handle_greatand_redi(const t_snode *redi_node,
		int io_index, char *word, int io[3])
{
	long	target_fd;

	(void) redi_node;
	if (io_index < 0)
		io_index = SH_STDOUT_INDEX;
	if (!ft_strcmp("-", word))
		return (io[io_index] = -1, 0);
	target_fd = ft_atol(word);
	if (target_fd < 0 || target_fd > SH_STDERR_INDEX)
		return ((sh_err1("invalid file descriptor")), -1);
	io[io_index] = io[target_fd];
	return (0);
}

/* TODO check if this works */
static int
	_cm_handle_lessand_redi(const t_snode *redi_node,
		int io_index, char *word, int io[3])
{
	long	target_fd;

	(void) redi_node;
	if (io_index < 0)
		io_index = SH_STDIN_INDEX;
	if (!ft_strcmp("-", word))
		return (io[io_index] = -1, 0);
	target_fd = ft_atol(word);
	if (target_fd < 0 || target_fd >= INT_MAX)
		return ((sh_err1("invalid file descriptor")), -1);
	if (target_fd >= SH_STDERR_INDEX)
		return (0);
	io[io_index] = target_fd;
	return (0);
}

static int
	_cm_handle_here_redi(const t_snode *redi_node, int io[3], int io_index)
{
	char	*str;
	int		here_read;

	if (io_index == -1)
		io_index = SH_STDIN_INDEX;
	str = cm_expand_str(&redi_node->childs[0]->here_content, NULL, ' ', 0);
	if (str == NULL)
		return (-1);
	here_read = _cm_create_and_write_here(str,
			redi_node->childs[0]->flags & flag_trim);
	io[io_index] = here_read;
	sh_fdctl(here_read, SH_FD_FIOCLEX, 1);
	free(str);
	return (0);
}

int
	_cm_handle_builtin_redi_node_noerr(const t_snode *node,
		char *filen, int io[3])
{
	int		io_index;

	io_index = _cm_redi_get_from_index(node);
	if (io_index < -1 || io_index == 3)
		return ((io_index < -1) * -1);
	if (node->type == sx_lessand)
		return (_cm_handle_lessand_redi(node, io_index, filen, io));
	if (node->type == sx_greatand)
		return (_cm_handle_greatand_redi(node, io_index, filen, io));
	if (node->type == sx_io_here)
		return (_cm_handle_here_redi(node, io, io_index));
	if (node->type == sx_less || node->type == sx_clobber
		|| node->type == sx_lessgreat)
	{
		if (io_index == -1)
			io_index = SH_STDIN_INDEX;
		return ((io[io_index] = _cm_open_file(filen,
					fl(node->type), 0644 * (node->type == sx_clobber
						|| node->type == sx_lessgreat)), 0) < 0);
	}
	if (sh_exists(filen) && sh()->clobber)
		return (sh_err1("cannot overwrite existing file"), -1);
	if (io_index == -1)
		io_index = SH_STDOUT_INDEX;
	return ((io[io_index] = _cm_open_file(filen, fl(node->type), 0644)) < 0);
}
