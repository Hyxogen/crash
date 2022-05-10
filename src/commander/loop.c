/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   loop.c                                           -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:59:48 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:59:48 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"
#include "minishell.h"

#include <stdio.h>

static int
	execute_for_body(const t_snode *fornode,
		char *name, char **list, const int io[SH_STDIO_SIZE])
{
	t_snode	*do_node;
	int		rc;

	do_node = fornode->childs[1];
	rc = 0;
	while (*list && !sh()->breaking)
	{
		sh()->continuing = 0;
		sh_setenv(name, *list, 0);
		rc = commandeer(do_node, io);
		list += 1;
	}
	if (sh()->breaking > 0)
	{
		sh()->breaking -= 1;
		rc = 0;
	}
	return (rc);
}

pid_t
	cm_for_clause(const t_snode *fornode, const int io[SH_STDIO_SIZE])
{
	char	*name;
	char	**list;
	int		rc;
	int		old_io[SH_STDIO_SIZE];

	sh()->loop_depth += 1;
	rc = 0;
	name = fornode->token.str;
	list = cm_word_list_to_array(fornode->childs[0]);
	command_setup_internal_redirects(fornode->childs[fornode->childs_size - 1],
		io, old_io);
	rc = execute_for_body(fornode, name, list, sh()->io);
	sh()->loop_depth -= 1;
	sh_strlst_clear(list);
	command_restore_internal_redirects(io, old_io);
	return (cm_convert_retcode(rc));
}

pid_t
	cm_while_until_clause(const t_snode *node, const int io[SH_STDIO_SIZE])
{
	int	rc;
	int	old_io[SH_STDIO_SIZE];

	sh()->loop_depth += 1;
	rc = -1;
	sh_assert(node->childs_size >= 3);
	command_setup_internal_redirects(node->childs[node->childs_size - 1],
		io, old_io);
	while (!commandeer(node->childs[0], sh()->io) == !(node->flags & flag_until)
		&& !sh()->breaking)
	{
		sh()->continuing = 0;
		rc = commandeer(node->childs[1], sh()->io);
	}
	if (sh()->breaking != 0)
	{
		sh()->breaking -= 1;
		rc = 0;
	}
	sh()->loop_depth -= 1;
	command_restore_internal_redirects(io, old_io);
	return (cm_convert_retcode(rc));
}
