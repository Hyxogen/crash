/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   die.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 15:02:23 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/24 10:49:39 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "memory.h"
#include <stdlib.h>

void
	pr_destroy(t_parser *pr)
{
	if (pr->current.id != tk_invalid)
		token_destroy(&pr->current);
	if (pr->next.id != tk_invalid)
		token_destroy(&pr->next);
}

void
	node_destroy_childs(t_snode *node)
{
	size_t	index;

	index = 0;
	while (index < node->childs_size)
	{
		node_destroy(node->childs[index]);
		index++;
	}
	free(node->childs);
}

void
	node_destroy(t_snode *node)
{
	if (!node)
		return ;
	node_destroy_childs(node);
	if (node->token.id != tk_invalid)
		token_destroy(&node->token);
	if (node->here_content.id != tk_invalid)
		token_destroy(&node->here_content);
	free(node);
}
