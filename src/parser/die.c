/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   die.c                                            -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:27 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:27 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "memory.h"
#include <stdlib.h>

void
	pr_destroy(t_parser *pr)
{
	token_destroy(&pr->current);
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
	node->refcount -= 1;
	if (node->refcount == 0)
	{
		if (node->type != sx_invalid)
			node_destroy_childs(node);
		token_destroy(&node->token);
		token_destroy(&node->here_content);
		free(node);
	}
}
