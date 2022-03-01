/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   command_extra.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:27:18 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/01 14:50:04 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int
	pr_cmd_prefix(t_parser *pr, t_snode *red, t_snode *ass, t_snode *cmd)
{
	(void) cmd;
	while (pr_io_redirect(pr, red)
		|| (pr->current_ret != 0
			&& pr_convert_ass(pr, pr->current, 1)
			&& pr_token(pr, ass, sx_assword, tk_assword)))
		continue ;
	if (red->childs_size != 0 || ass->childs_size != 0)
		return (1);
	return (0);
}

void
	pr_cmd_suffix(t_parser *pr, t_snode *red, t_snode *ass, t_snode *cmd)
{
	(void) ass;
	while (pr_io_redirect(pr, red)
		|| pr_token(pr, cmd, sx_word, tk_word))
		continue ;
}
