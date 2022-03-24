/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   subshell.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:19:35 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/24 10:01:08 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int
	pr_subshell(t_parser *pr, t_snode *parent)
{
	if (pr_token(pr, NULL, sx_none, op_lparen))
	{
		if (pr_compound_list(pr, parent))
		{
			if (pr_token(pr, NULL, sx_none, op_rparen))
			{
				parent->childs[parent->childs_size - 1]->flags |= flag_subshell;
				return (1);
			}
		}
		pr->lexer->error = SH_PR_UNEXTOKEN;
	}
	return (0);
}
