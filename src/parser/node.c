/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   node.c                                           -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:23:23 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:23:23 by csteenvo            :      ..            */
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
	if (node->childs_size == node->childs_capacity)
		node_resize_childs(node, node->childs_capacity + SH_DEF_CHILD_SIZE);
	node->childs[node->childs_size] = child;
	node->childs_size++;
}

void
	node_move(t_snode *dest, t_snode *src)
{
	if (dest == src)
		return ;
	ft_memcpy(dest, src, sizeof(*src));
	ft_memset(src, 0, sizeof(*src));
	src->type = sx_invalid;
}
