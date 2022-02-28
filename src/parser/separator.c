/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   separator.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:17:52 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/28 10:18:15 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int
	pr_seperator_op(t_parser *pr, t_snode *parent)
{
	if (pr_token(pr, NULL, sx_and, op_and))
	{
		parent->flags |= flag_and;
		return (1);
	}
	if (pr_token(pr, NULL, sx_semicolon, op_semi))
	{
		parent->flags |= flag_semi;
		return (1);
	}
	return (0);
}

int
	pr_sequential_sep(t_parser *pr, t_snode *parent)
{
	(void) parent;
	if (!pr_token(pr, NULL, sx_none, op_semi)
		&& !pr_token(pr, NULL, sx_none, tk_newline))
		return (0);
	while (pr_token(pr, NULL, sx_newline, tk_newline))
		continue ;
	return (1);
}
