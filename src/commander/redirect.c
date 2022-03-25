/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   redirect.c                                       -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:22:27 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:22:27 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"

#include <libft.h>
#include <stdio.h>
#include <limits.h>
#include <fcntl.h>

int
	_cm_open_file_out(const char *filen, int fd, int append)
{
	int	result_fd;

	result_fd = sh_open(filen, O_WRONLY | O_CREAT | ((!!append) * O_APPEND) | ((!append) * O_TRUNC), 0644);
	if (fd >= 0 && result_fd != fd)
	{
		sh_dup2(result_fd, fd);
		sh_close(result_fd);
		result_fd = fd;
	}
	return (result_fd);
}

int
	_cm_open_file_in(const char *filen, int fd)
{
	int	result_fd;

	result_fd = sh_open(filen, O_RDONLY, 0);
	if (fd >= 0 && result_fd != fd)
	{
		sh_dup2(result_fd, fd);
		sh_close(result_fd);
		result_fd = fd;
	}
	return (result_fd);
}

int
	_cm_handle_redi_node(t_snode *redi_node)
{
	int			target_fd;
	const char	*filen;
	t_snode		*file_node;

	target_fd = -1;
	file_node = redi_node->childs[0];
	if (redi_node->childs_size >= 2)
	{
		target_fd = ft_atol(redi_node->childs[0]->token.str);
		if (target_fd < 0 || target_fd >= INT_MAX)
		{
			fprintf(stderr, "CraSH: Invalid file descriptor\n");
			return (0);
		}
		file_node = redi_node->childs[1];
		filen = cm_expand(&redi_node->childs[1]->token);
	}
	filen = cm_expand(&file_node->token);
	if (redi_node->type == sx_less)
	{
		if (target_fd == -1)
				target_fd = 0;
		return (_cm_open_file_in(filen, target_fd) < 0);
	}
	else if (redi_node->type == sx_great)
	{
		if (target_fd == -1)
			target_fd = 1;
		return (_cm_open_file_out(filen, target_fd, 0) < 0);
	}
	if (target_fd == -1)
			target_fd = 1;
	return (_cm_open_file_out(filen, target_fd, 1) < 0);
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
		rc  = _cm_handle_redi_node(node);
		if (rc)
		{
			fprintf(stdout, "CraSH: Failed to setup redirect\n");
			return (rc);
		}
		index++;
	}
	return (0);
}
