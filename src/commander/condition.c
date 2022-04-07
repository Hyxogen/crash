#include "commander.h"

#include "memory.h"
#include <libft.h>
#include <string.h>


#include <stdio.h>

/* TODO setup redirects */
pid_t
	cm_if_clause(t_minishell *sh, t_snode *ifnode, const int io[3])
{
	int	rc;

	sh_assert(ifnode->type == sx_if_clause);
	rc = commandeer(sh, ifnode->childs[0], io);
	if (!rc)
		return (cm_convert_retcode(commandeer(sh, ifnode->childs[1], io)));
	if (ifnode->childs_size >= 3 && ifnode->childs[2]->type == sx_if_clause)
		return (cm_if_clause(sh, ifnode->childs[2], io));
	else if (ifnode->childs_size >= 3)
		return (cm_convert_retcode(commandeer(sh, ifnode->childs[2], io)));
	return (cm_convert_retcode(0));
}

/* TODO handle lists */
/* TODO handle pattern matching */
static int
	_cm_strlst_cmp(char **lhs, char **rhs)
{
	char	*lhs_str;
	char	*rhs_str;
	char	*escape_info;
	int		match;

	lhs_str = *lhs;
	rhs_str = *rhs;
	
	if (lhs_str == rhs_str)
		return (0);
	escape_info = sh_safe_malloc(ft_strlen(rhs_str)); /* TODO setup actual escape info */
	match = match_pattern(lhs_str, rhs_str, escape_info);
	printf("lhs:\"%s\" rhs:\"%s\" match:%d\n", lhs_str, rhs_str, match);
	return (!match);
}

pid_t
	cm_case_clause(t_minishell *sh, t_snode *node, const int io[3])
{
	size_t	clauses;
	size_t	index;
	char	**lhs;
	char	**rhs;
	int		case_io[3];

	sh_assert(node->type == sx_case_clause);
	clauses = node->childs_size - 1;
	lhs = cm_expand(sh, &node->token); // TODO use cm_expand_str instead
	index = 0;
	ft_memcpy(case_io, io, sizeof(int) * 3);
	_cm_setup_builtin_redirects(sh, node->childs[node->childs_size - 1], case_io);
	while (index < clauses)
	{
		rhs = cm_expand(sh, &node->childs[index]->token); // TODO use cm_expand_str instead
		if (!_cm_strlst_cmp(lhs, rhs))
			return (cm_convert_retcode(commandeer(sh, node->childs[index], io)));
		index++;
	}
	return (cm_convert_retcode(0));
}
