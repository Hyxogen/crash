/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   die.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 14:51:59 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/01 16:27:55 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parser.h"
#include <stdlib.h>

void
	tpart_destroy(t_tpart *part)
{
	if (part->id == lx_normal || part->id == lx_backtick)
	{
		free(part->data);
	}
	else if (part->id == lx_command)
	{
		node_destroy(part->data);
	}
	else if (part->id == lx_parameter || part->id == lx_arithmetic)
	{
		token_destroy(part->data);
		free(part->data);
	}
}

void
	token_destroy(t_token *tok)
{
	size_t	i;

	i = 0;
	free(tok->str);
	while (i < tok->count)
	{
		tpart_destroy(&tok->parts[i]);
		i += 1;
	}
	free(tok->parts);
}
