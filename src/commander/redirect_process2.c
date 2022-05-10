/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   redirect_process2.c                              -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:05 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:05 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"

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

/* TODO check if correct */
static int
	_cm_handle_lessand_redi(const t_snode *redi_node, int from_fd, char *word)
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
	_cm_handle_greatand_redi(const t_snode *redi_node, int from_fd, char *word)
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

static int
	_cm_handle_here_redi(const t_snode *redi_node, int from_fd)
{
	char	*str;

	if (from_fd == -1)
		from_fd = STDIN_FILENO;
	str = cm_expand_str(&redi_node->childs[0]->here_content, NULL, ' ', 0);
	if (str == NULL)
		return (-1);
	sh_dup2(_cm_create_and_write_here(str,
			redi_node->childs[0]->flags & flag_trim), from_fd);
	free(str);
	return (0);
}

int
	_cm_handle_process_node_noerr(const t_snode *node, char *filen)
{
	int		from_fd;

	from_fd = _cm_redi_get_from(node);
	if (from_fd < -1)
		return (-1);
	if (node->type == sx_lessand)
		return (_cm_handle_lessand_redi(node, from_fd, filen));
	if (node->type == sx_greatand)
		return (_cm_handle_greatand_redi(node, from_fd, filen));
	if (node->type == sx_io_here)
		return (_cm_handle_here_redi(node, from_fd));
	if (node->type == sx_less || node->type == sx_clobber
		|| node->type == sx_lessgreat)
	{
		if (from_fd == -1)
			from_fd = SH_STDIN_INDEX;
		return (_cm_open_file(filen, from_fd, fl(node->type),
				0644 * (node->type == sx_clobber
					|| node->type == sx_lessgreat)) < 0);
	}
	if (sh_exists(filen) && sh()->clobber)
		return (sh_err1("cannot overwrite existing file"), -1);
	if (from_fd == -1)
		from_fd = SH_STDOUT_INDEX;
	return (_cm_open_file(filen, from_fd, fl(node->type), 0644) < 0);
}
