/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   node_int.c                                       -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:23:24 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:23:24 by csteenvo            :      ..            */
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
	node->childs = NULL;
	node->childs_capacity = 0;
	node->childs_size = 0;
	node->flags = 0;
	token_init(&node->token);
	token_init(&node->here_content);
	return (node);
}
