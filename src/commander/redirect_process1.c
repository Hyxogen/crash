/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   redirect_process1.c                              -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:04 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:04 by csteenvo            :      ..            */
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

int
	_cm_redi_get_from(const t_snode *redi_node)
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

static int
	_cm_handle_process_redi_node(const t_snode *redi_node)
{
	char	**filen;
	int		return_code;

	if (redi_node->childs_size == 0)
		return (sh_err1("no file specified"), 1);
	filen = cm_wildcard_expand(&redi_node->childs[0]->token);
	if (!filen)
		return (-1);
	if (!*filen || *(filen + 1))
	{
		sh_strlst_clear(filen);
		return (sh_err1("ambiguous redirect"), 1);
	}
	return_code = _cm_handle_process_node_noerr(redi_node, *filen);
	sh_strlst_clear(filen);
	return (return_code);
}

int
	_cm_setup_process_redirects(const t_snode *redi_list)
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
		rc = _cm_handle_process_redi_node(node);
		if (rc)
			return (rc);
		index++;
	}
	return (0);
}
