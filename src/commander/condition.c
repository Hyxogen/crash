#include "commander.h"

#include "minishell.h"
#include "memory.h"
#include <libft.h>
#include <string.h>
#include <stdlib.h>

static pid_t
	cm_elif_clause(const t_snode *node, const int io[3])
{
	int	statement_ret;
	int	body_ret;

	SH_ASSERT(node->type == sx_if_clause);
	statement_ret = commandeer(node->childs[0], io);
	body_ret = 0;
	if (!statement_ret)
		body_ret = commandeer(node->childs[1], io);
	else if (node->childs_size >= 3
		&& node->childs[2]->type == sx_if_clause)
		body_ret = cm_if_clause(node->childs[2], io);
	else if (node->childs_size >= 3)
		return (cm_convert_retcode(commandeer(node->childs[2], io)));
	return (cm_convert_retcode(body_ret));
}

pid_t
	cm_if_clause(const t_snode *node, const int io[3])
{
	int	old_io[3];
	int	statement_ret;
	int	body_ret;

	SH_ASSERT(node->type == sx_if_clause);
	command_setup_internal_redirects(node->childs[node->childs_size - 1],
		io, old_io);
	statement_ret = commandeer(node->childs[0], sh()->io);
	body_ret = 0;
	if (!statement_ret)
		body_ret = commandeer(node->childs[1], sh()->io);
	else if (node->childs_size >= 3
		&& node->childs[2]->type == sx_if_clause)
		body_ret = cm_elif_clause(node->childs[2], sh()->io);
	else if (node->childs_size >= 3)
		body_ret = commandeer(node->childs[2], sh()->io);
	command_restore_internal_redirects(io, old_io);
	return (cm_convert_retcode(body_ret));
}

static int
	_cm_strlst_cmp(char *lhs, t_token *tok)
{
	char			*rhs_str;
	int				*escape_info;
	int				match;
	t_pattern_node	*pattern;

	escape_info = NULL;
	rhs_str = cm_expand_str(tok, &escape_info, ' ', 1);
	if (rhs_str == NULL)
		return (-1);
	pattern = pattern_compile(rhs_str, escape_info);
	match = pattern_match(lhs, pattern, 0);
	pattern_destroy(pattern);
	free(escape_info);
	free(rhs_str);
	return (!match);
}

static int
	execute_case_clause(const t_snode *node, char *lhs, size_t clauses)
{
	int		rc;
	int		cmp;
	size_t	index;

	index = 0;
	rc = 0;
	while (index < clauses)
	{
		cmp = _cm_strlst_cmp(lhs, &node->childs[index]->token);
		if (!cmp)
		{
			rc = commandeer(node->childs[index], sh()->io);
			break ;
		}
		else if (cmp < 0)
		{
			rc = 1;
			break ;
		}
		index++;
	}
	return (rc);
}

pid_t
	cm_case_clause(const t_snode *node, const int io[3])
{
	size_t	clauses;
	char	*lhs;
	int		old_io[3];
	int		rc;

	SH_ASSERT(node->type == sx_case_clause);
	clauses = node->childs_size - 1;
	lhs = cm_expand_str(&node->token, NULL, ' ', 1);
	if (!lhs)
		return (cm_convert_retcode(1));
	command_setup_internal_redirects(node->childs[node->childs_size - 1],
		io, old_io);
	rc = execute_case_clause(node, lhs, clauses);
	command_restore_internal_redirects(io, old_io);
	free(lhs);
	return (cm_convert_retcode(rc));
}
