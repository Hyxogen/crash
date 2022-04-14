/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   redirect.c                                       -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:22:30 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:22:30 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"
#include "minishell.h"

#include <libft.h>
#include <stdio.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

static int
	_cm_open_file(const char *filen, int fd, int flags, int mode)
{
	int	result_fd;

	result_fd = sh_open(filen, flags, mode);
	if (fd >= 0 && result_fd != fd)
	{
		sh_dup2(result_fd, fd);
		sh_close(result_fd);
		result_fd = fd;
	}
	return (result_fd);
}

static int
	_cm_redi_get_from(t_snode *redi_node)
{
	int	from_fd;

	from_fd = -1;
	if (redi_node->token.id == tk_ionumber)
	{
		from_fd = ft_atol(redi_node->token.str);
		if (from_fd < 0 || from_fd >= INT_MAX)
		{
			sh_err1("invalid file descriptor");
			return (0);
		}
	}
	return (from_fd);
}

/* TODO check if correct */
static int
	_cm_handle_lessand_redi(t_snode *redi_node, int from_fd, char *word)
{
	long	target_fd;

	(void) redi_node;
	if (from_fd < 0)
		from_fd = STDIN_FILENO;
	if (!ft_strcmp("-", word))
		return (sh_close(from_fd), 0);
	target_fd = ft_atol(word);
	if (target_fd < 0 || target_fd >= INT_MAX)
		return (sh_err1("invalid file descriptor"), -1);
	return (sh_dup2(from_fd, target_fd) < 0);
}

static int
	_cm_handle_greatand_redi(t_snode *redi_node, int from_fd, char *word)
{
	long	target_fd;

	(void) redi_node;
	if (from_fd < 0)
		from_fd = STDOUT_FILENO;
	if (!ft_strcmp("-", word))
		return (sh_close(from_fd), 0);
	target_fd = ft_atol(word);
	if (target_fd < 0 || target_fd >= INT_MAX)
		return (sh_err1("invalid file descriptor"), -1);
	return (sh_dup2(target_fd, from_fd) < 0);
}

/* TODO check if here docs can be redirected to a specific file descriptor */
static int
	_cm_handle_here_redi(t_snode *redi_node)
{
	char	*str;
	int		here_pipe[2];
	pid_t	pid;

	str = cm_expand_str(&redi_node->childs[0]->here_content, NULL, ' ');
	if (str == NULL)
		return (-1);
	sh_pipe(here_pipe);
	sh_dup2(here_pipe[0], STDIN_FILENO);
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
	_cm_check_clobber(const char *filen)
{
	(void) filen;
	return (0);
}

static int
	_cm_handle_redi_node_noerr(t_snode *redi_node, char *filen)
{
	int		from_fd;

	from_fd = _cm_redi_get_from(redi_node);
	if (from_fd < -1)
		return (-1);
	if (redi_node->type == sx_lessand)
		return (_cm_handle_lessand_redi(redi_node, from_fd, filen));
	if (redi_node->type == sx_greatand)
		return (_cm_handle_greatand_redi(redi_node, from_fd, filen));
	if (redi_node->type == sx_io_here)
		return (_cm_handle_here_redi(redi_node));
	if (redi_node->type == sx_less || redi_node->type == sx_clobber || redi_node->type == sx_lessgreat)
	{
		if (from_fd == -1)
			from_fd = SH_STDIN_INDEX;
		return  (_cm_open_file(filen, from_fd,
			_cm_get_redi_flags(redi_node->type),
			0644 * (redi_node->type == sx_clobber || redi_node->type == sx_lessgreat)) < 0);
	}
	if (sh_exists(filen) && _cm_check_clobber(filen))
		return (sh_err1("cannot overwrite existing file"), -1);
	if (from_fd == -1)
		from_fd = SH_STDOUT_INDEX;
	return (_cm_open_file(filen, from_fd,
		_cm_get_redi_flags(redi_node->type), 0644) < 0);
}

static int
	_cm_handle_redi_node(t_snode *redi_node)
{
	char	**filen;

	if (redi_node->childs_size == 0)
		return (sh_err1("no file specified"), 1);
	filen = cm_expand(&redi_node->childs[0]->token);
	if (!filen)
		return (-1);
	if (!*filen || *(filen + 1))
		return (sh_err1("ambiguous redirect"), 1);
	return (_cm_handle_redi_node_noerr(redi_node, *filen));
}

int
	_cm_setup_process_redirects(t_snode *redi_list)
{
	t_snode	*node;
	size_t	size;
	size_t	index;
	int		rc;

	index = 0;
	size = redi_list->childs_size;
	while (index < size)
	{
		node = redi_list->childs[index];
		rc = _cm_handle_redi_node(node);
		if (rc)
			return (rc);
		index++;
	}
	return (0);
}
