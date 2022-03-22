/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   node_int.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:23:27 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/28 13:16:51 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "memory.h"

/*shrinking is undefined*/
void
	node_resize_childs(t_snode *node, size_t newsize)
{
	node->childs = sh_safe_realloc(
			node->childs,
			sizeof(t_snode*) * node->childs_capacity,
			sizeof(t_snode) * newsize);
	node->childs_capacity = newsize;
}

t_snode
	*node_init(t_snode *node, t_syntax_id syn_id)
{
	node->type = syn_id;
	node->parent = NULL;
	node->childs = NULL;
	node->childs_capacity = 0;
	node->childs_size = 0;
	node->token = NULL;
	node->flags = 0;
	node->here_content = NULL;
	return (node);
}