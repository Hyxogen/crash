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

#include <libft.h>
#include <stdio.h>
#include <limits.h>
#include <fcntl.h>

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
	_cm_redi_get_from(t_minishell *sh, t_snode *redi_node)
{
	int	from_fd;

	(void) sh;
	from_fd = -1;
	if (redi_node->token.id == tk_ionumber)
	{
		from_fd = ft_atol(redi_node->token.str);
		if (from_fd < 0 || from_fd >= INT_MAX)
		{
			fprintf(stderr, "CraSH: Invalid file descriptor\n");
			return (0);
		}
	}
	return (from_fd);
}
/*
int
	_cm_handle_redi_node(t_minishell *sh, t_snode *redi_node)
{
	int			target_fd;
	const char	**filen;
	t_snode		*file_node;

	target_fd = -1;
	file_node = redi_node->childs[0];
	if (redi_node->token.id == tk_ionumber)
	{
		target_fd = ft_atol(redi_node->token.str);
		if (target_fd < 0 || target_fd >= INT_MAX)
		{
			fprintf(stderr, "CraSH: Invalid file descriptor\n");
			return (0);
		}
	}
	filen = cm_expand(sh, &file_node->token); // TODO Implement new expand system
	if (!filen || !*filen || *(filen + 1))
	{
		fprintf(stderr, "CraSH: Ambigious redirect\n");
		return (0);
	}
	target_fd = (target_fd != -1) * target_fd + (redi_node->type != sx_less);
	if (redi_node->type == sx_less)
		return (_cm_open_file_in(filen, target_fd) < 0);
	else if (redi_node->type == sx_great)
		return (_cm_open_file_out(filen, target_fd, 0) < 0);
	return (_cm_open_file_out(filen, target_fd, 1) < 0);
}
*/

/* TODO implement */
static int
	_cm_handle_lessand_redi(t_minishell *sh, t_snode *redi_node, int from_fd)
{
	(void) sh;
	(void) redi_node;
	(void) from_fd;
	return (0);
}

/* TODO implement */
static int
	_cm_handle_greatand_redi(t_minishell *sh, t_snode *redi_node, int from_fd)
{
	(void) sh;
	(void) redi_node;
	(void) from_fd;
	return (0);
}

/* TODO implement */
static int
	_cm_handle_here_redi(t_minishell *sh, t_snode *redi_node)
{
	(void) sh;
	(void) redi_node;
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
	return (0);
}

/* TODO: implement clobber check */
static int
	_cm_check_clobber(t_minishell *sh, const char *filen)
{
	(void) sh;
	(void) filen;
	return (0);
}

static int
	_cm_handle_redi_node(t_minishell *sh, t_snode *redi_node)
{
	int		from_fd;
	char	**filen;

	if (redi_node->type == sx_io_here)
		return (_cm_handle_here_redi(sh, redi_node));
	from_fd = _cm_redi_get_from(sh, redi_node);
	if (redi_node->type == sx_lessand)
		return (_cm_handle_lessand_redi(sh, redi_node, from_fd));
	if (redi_node->type == sx_greatand)
		return (_cm_handle_greatand_redi(sh, redi_node, from_fd));
	if (redi_node->childs_size == 0)
		return (fprintf(stderr, "CraSH: no file specified\n"), 1);
	filen = cm_expand(sh, &redi_node->childs[0]->token);
	if (!filen || !*filen || *(filen + 1))
		return (fprintf(stderr, "CraSH: ambigious redirect\n"), 1);
	from_fd = (from_fd != -1) * from_fd + ((redi_node->type != sx_less) && (redi_node->type != sx_lessgreat));
	if (redi_node->type == sx_clobber || redi_node->type == sx_lessgreat)
		return (_cm_open_file(*filen, from_fd, _cm_get_redi_flags(redi_node->type), 0644) < 0);
	if (redi_node->type == sx_less)
		return (_cm_open_file(*filen, from_fd, _cm_get_redi_flags(redi_node->type), 0) < 0);
	if (_cm_check_clobber(sh, *filen))
		return (fprintf(stderr, "CraSH: cannot overwrite existing file"), 1);
	if (redi_node->type == sx_great || redi_node->type == sx_dgreat)
		return (_cm_open_file(*filen, from_fd, _cm_get_redi_flags(redi_node->type), 0644) < 0);
	return (-1);
}

int
	_cm_setup_redirects(t_minishell *sh, t_snode *redi_list)
{
	t_snode	*node;
	size_t	size;
	size_t	index;
	int		rc;

	(void) sh;
	index = 0;
	size = redi_list->childs_size;
	while (index < size)
	{
		node = redi_list->childs[index];
		rc  = _cm_handle_redi_node(sh, node);
		if (rc)
		{
			fprintf(stdout, "CraSH: Failed to setup redirect\n");
			return (rc);
		}
		index++;
	}
	return (0);
}
