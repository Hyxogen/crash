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
	return (result_fd);
}

static int
	_cm_redi_get_from_index(t_snode *redi_node)
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
	_cm_handle_greatand_redi(t_snode *redi_node, int io_index, char *word, int io[3])
{
	long	target_fd;

	(void) redi_node;
	if (io_index < 0)
		io_index = SH_STDOUT_INDEX;
	if (!ft_strcmp("-", word))
		return (io[io_index] = -1, 0);
	target_fd = ft_atol(word);
	if (target_fd < 0 || target_fd > SH_STDERR_INDEX)
		return (sh_err1("invalid file descriptor")), -1;
	io[io_index] = io[target_fd];
	return (0);
}

/* TODO check if this works */
static int
	_cm_handle_lessand_redi(t_snode *redi_node, int io_index, char *word, int io[3])
{
	long	target_fd;

	(void) redi_node;
	if (io_index < 0)
		io_index = SH_STDOUT_INDEX;
	if (!ft_strcmp("-", word))
		return (io[io_index] = -1, 0);
	target_fd = ft_atol(word);
	if (target_fd < 0 || target_fd >= INT_MAX)
		return (sh_err1("invalid file descriptor")), -1;
	if (target_fd >= SH_STDERR_INDEX)
		return (0);
	io[io_index] = target_fd;
	return (0);
}

static int
	_cm_handle_here_redi(t_snode *redi_node, int io[3])
{
	char	*str;
	int		here_pipe[2];
	pid_t	pid;

	str = cm_expand_str(&redi_node->childs[0]->here_content, NULL, ' ');
	if (str == NULL)
		return (-1);
	sh_pipe(here_pipe);
	io[SH_STDIN_INDEX] = here_pipe[0];
	pid = sh_fork();
	if (pid == 0)
	{
		sh_close(here_pipe[0]);
		sh_write(here_pipe[1], str, ft_strlen(str));
		free(str);
		sh_close(here_pipe[1]);
		exit(EXIT_SUCCESS);
	}
	sh_close(here_pipe[1]);
	sh_fdctl(here_pipe[0], SH_FD_FIOCLEX, 1);
	free(str);
	return (0);
}

static int
	_cm_get_redi_flags(t_syntax_id type)
{
	if (type == sx_great || type == sx_lessgreat)
		return (O_WRONLY | O_TRUNC | O_CREAT);
	if (type == sx_dgreat)
		return (O_WRONLY | O_APPEND | O_CREAT);
	if (type == sx_less)
		return (O_RDONLY);
	if (type == sx_lessgreat)
		return (O_RDWR | O_TRUNC | O_CREAT);
	return (0);
}

/* TODO: implement clobber check */
static int
	_cm_check_clobber(void)
{
	return (0);
}

static int
	_cm_handle_redi_node_noerr(t_snode *redi_node, char *filen, int io[3])
{
	int		io_index;

	io_index = _cm_redi_get_from_index(redi_node);
	if (io_index < -1 || io_index == 3)
		return ((io_index < -1) * -1);
	if (redi_node->type == sx_lessand)
		return (_cm_handle_lessand_redi(redi_node, io_index, filen, io));
	if (redi_node->type == sx_greatand)
		return (_cm_handle_greatand_redi(redi_node, io_index, filen, io));
	if (redi_node->type == sx_io_here)
		return (_cm_handle_here_redi(redi_node, io));
	if (redi_node->type == sx_less || redi_node->type == sx_clobber || redi_node->type == sx_lessgreat)
	{
		if (io_index == -1)
			io_index = SH_STDIN_INDEX;
		return  ((io[io_index] = _cm_open_file(filen,
			_cm_get_redi_flags(redi_node->type),
			0644 * (redi_node->type == sx_clobber || redi_node->type == sx_lessgreat)), 0) < 0);
	}
	if (sh_exists(filen) && _cm_check_clobber())
		return (sh_err1("cannot overwrite existing file"), -1);
	if (io_index == -1)
		io_index = SH_STDOUT_INDEX;
	return ((io[io_index] = _cm_open_file(filen,
		_cm_get_redi_flags(redi_node->type), 0644)) < 0);
}

static int
	_cm_handle_redi_node(t_snode *redi_node, int io[3])
{
	char	**filen;

	if (redi_node->childs_size == 0)
		return (sh_err1("no file specified"), -1);
	sh_assert(redi_node->childs[0]->token.id != tk_invalid);
	filen = cm_expand(&redi_node->childs[0]->token);
	if (!filen)
		return (-1);
	if (!*filen || *(filen + 1))
		return (sh_err1("ambigious redirect"), -1);
	return (_cm_handle_redi_node_noerr(redi_node, *filen, io));
}

int
	_cm_setup_builtin_redirects(t_snode *redi_list, int io[3])
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
		rc  = _cm_handle_redi_node(node, io);
		if (rc)
			return (rc);
		index++;
	}
	return (0);
}
