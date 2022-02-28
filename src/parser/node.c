/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   node.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:15:17 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/28 14:42:56 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "memory.h"
#include <stdlib.h>

#define SH_DEF_CHILD_SIZE 10

t_snode
	*snode(t_syntax_id syn_id)
{
	t_snode	*node;

	node = sh_safe_malloc(sizeof(*node));
	node_init(node, syn_id);
	return (node);
}

void
	node_add_child(t_snode *node, t_snode *child)
{
	child->parent = node;
	if (node->childs_size == node->childs_capacity)
		node_resize_childs(node, node->childs_capacity + SH_DEF_CHILD_SIZE);
	node->childs[node->childs_size] = child;
	node->childs_size++;
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
	if (node->token != NULL)
	{
		token_destroy(node->token);
		free(node->token);
	}
	if (node->here_content != NULL)
	{
		token_destroy(node->here_content);
		free(node->here_content);
	}
	free(node);
}
