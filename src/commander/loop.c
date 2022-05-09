#include "commander.h"
#include "minishell.h"

#include <stdio.h>
/*
TODO: Check this:
> for name in 1 2 3 4; do
> ./fdcheck
> done < src/main.c
*/

/* TODO implement no wordlist */
/* TODO setup redirects */

static int
	execute_for_body(const t_snode *fornode, char *name, char **list, const int io[SH_STDIO_SIZE])
{
	t_snode	*do_node;
	int		rc;

	do_node = fornode->childs[1];
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
	cm_for_clause(const t_snode *fornode, const int io[3])
{
	char	*name;
	char	**list;
	int		rc;
	int		for_io[3];

	sh()->loop_depth += 1;
	rc = 0;
	name = fornode->token.str;
	list = cm_word_list_to_array(fornode->childs[0]);
	ft_memcpy(for_io, io, sizeof(for_io));
	_cm_setup_builtin_redirects(fornode->childs[fornode->childs_size - 1], for_io); /* TODO use new redirect system (see condition.c) */
	rc = execute_for_body(fornode, name, list, for_io);
	sh()->loop_depth -= 1;
	sh_strlst_clear(list);
	return (cm_convert_retcode(rc));
}

pid_t
	cm_while_until_clause(const t_snode *node, const int io[3])
{
	int	rc;
	int	loop_io[3];

	sh()->loop_depth += 1;
	rc = -1;
	sh_assert(node->childs_size >= 3);
	ft_memcpy(loop_io, io, sizeof(loop_io));
	_cm_setup_builtin_redirects(node->childs[node->childs_size - 1], loop_io); /* TODO use new redirect system (see condition.c) */
	while (!commandeer(node->childs[0], loop_io) == !(node->flags & flag_until)
		&& !sh()->breaking)
	{
		sh()->continuing = 0;
		rc = commandeer(node->childs[1], loop_io);
	}
	if (sh()->breaking != 0)
	{
		sh()->breaking -= 1;
		rc = 0;
	}
	sh()->loop_depth -= 1;
	return (cm_convert_retcode(rc));
}
