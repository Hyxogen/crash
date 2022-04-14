#include "commander.h"
#include "minishell.h"
#include "memory.h"
#include <libft.h>
#include <string.h>


#include <stdio.h>

static pid_t
	cm_elif_clause(t_snode *ifnode, const int io[3])
{
	int	statement_ret;
	int	body_ret;

	sh_assert(ifnode->type == sx_if_clause);
	statement_ret = commandeer(ifnode->childs[0], io);
	body_ret = 0;
	if (!statement_ret)
		body_ret = commandeer(ifnode->childs[1], io);
	else if (ifnode->childs_size >= 3 && ifnode->childs[2]->type == sx_if_clause)
		body_ret = cm_if_clause(ifnode->childs[2], io);
	else if (ifnode->childs_size >= 3)
		return (cm_convert_retcode(commandeer(ifnode->childs[2], io)));
	return (cm_convert_retcode(body_ret));
}

/* TODO setup redirects */
pid_t
	cm_if_clause(t_snode *ifnode, const int io[3])
{
	int	if_io[3];
	int	statement_ret;
	int	body_ret;

	sh_assert(ifnode->type == sx_if_clause);
	ft_memcpy(if_io, io, sizeof(int) * 3);
	_cm_setup_builtin_redirects(ifnode->childs[ifnode->childs_size - 1], if_io);
	statement_ret = commandeer(ifnode->childs[0], if_io);
	body_ret = 0;
	if (!statement_ret)
		body_ret = commandeer(ifnode->childs[1], if_io);
	else if (ifnode->childs_size >= 3 && ifnode->childs[2]->type == sx_if_clause)
		body_ret = cm_elif_clause(ifnode->childs[2], if_io);
	else if (ifnode->childs_size >= 3)
		body_ret = commandeer(ifnode->childs[2], if_io);
	cm_close_nstd_nred(io, if_io);
	return (cm_convert_retcode(body_ret));
}

static int
	_cm_strlst_cmp(char *lhs, t_token *tok)
{
	char	*rhs_str;
	int		*escape_info;
	int		match;

	escape_info = NULL;
	
	rhs_str = cm_expand_str(tok, &escape_info, ' ');
	if (rhs_str == NULL)
		return (-1);
	match = pattern_match(lhs, rhs_str, escape_info);
	return (!match);
}

pid_t
	cm_case_clause(t_snode *node, const int io[3])
{
	size_t	clauses;
	size_t	index;
	char	*lhs;
	int		case_io[3];
	int		cmp;
	int		rc;

	rc = 0;
	sh_assert(node->type == sx_case_clause);
	clauses = node->childs_size - 1;
	lhs = cm_expand_str(&node->token, NULL, ' ');
	if (!lhs)
		return (cm_convert_retcode(1));
	index = 0;
	ft_memcpy(case_io, io, sizeof(int) * 3);
	_cm_setup_builtin_redirects(node->childs[node->childs_size - 1], case_io);
	while (index < clauses)
	{
		cmp = _cm_strlst_cmp(lhs, &node->childs[index]->token);
		if (!cmp)
		{
			rc = commandeer(node->childs[index], case_io);
			break;
		}
		else if (cmp < 0)
		{
			rc =  1;
			break;
		}
		index++;
	}
	cm_close_nstd_nred(io, case_io);
	return (cm_convert_retcode(rc));
}
