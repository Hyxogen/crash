/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   die.c                                            -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:23:10 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:23:10 by csteenvo            :      ..            */
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
