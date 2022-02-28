/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   die.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 15:02:23 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/28 15:04:08 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

void
	pr_destroy(t_parser *pr)
{
	if (pr->current_ret)
	{
		token_destroy(pr->current);
		free(pr->current);
	}
	if (pr->next_ret)
	{
		token_destroy(pr->next);
		free(pr->next);
	}
}
