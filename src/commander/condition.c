#include "commander.h"

#include "memory.h"
#include <libft.h>
#include <string.h>


#include <stdio.h>

/* TODO setup redirects */
pid_t
	cm_if_clause(t_snode *ifnode, const int io[3], int closefd)
{
	int	rc;

	sh_assert(ifnode->type == sx_if_clause);
	rc = commandeer(ifnode->childs[0], io);
	if (!rc)
		return (cm_convert_retcode(commandeer(ifnode->childs[1], io)));
	if (ifnode->childs_size >= 3 && ifnode->childs[2]->type == sx_if_clause)
		return (cm_if_clause(ifnode->childs[2], io, closefd));
	else if (ifnode->childs_size >= 3)
		return (cm_convert_retcode(commandeer(ifnode->childs[2], io)));
	return (cm_convert_retcode(0));
}

static int
	_cm_strlst_cmp(char *lhs, t_token *tok)
{
	char	*rhs_str;
	int		*escape_info;
	int		match;

	escape_info = NULL;
	
	rhs_str = cm_expand_str(tok, &escape_info, ' ');
	match = pattern_match(lhs, rhs_str, escape_info);
	return (!match);
}

pid_t
	cm_case_clause(t_snode *node, const int io[3], int closefd)
{
	size_t	clauses;
	size_t	index;
	char	*lhs;
	int		case_io[3];

	(void) closefd;
	sh_assert(node->type == sx_case_clause);
	clauses = node->childs_size - 1;
	lhs = cm_expand_str(&node->token, NULL, ' ');
	index = 0;
	ft_memcpy(case_io, io, sizeof(int) * 3);
	_cm_setup_builtin_redirects(node->childs[node->childs_size - 1], case_io);
	while (index < clauses)
	{
		if (!_cm_strlst_cmp(lhs, &node->childs[index]->token))
			return (cm_convert_retcode(commandeer(node->childs[index], io)));
		index++;
	}
	return (cm_convert_retcode(0));
}
