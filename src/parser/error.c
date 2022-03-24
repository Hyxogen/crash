/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/22 13:17:19 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/24 10:08:21 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"


#include <stdio.h>

int
	pr_error_token(t_parser *pr, t_snode *parent,
		t_syntax_id syn_id, t_token_id tk_id)
{
	t_snode	*node;

	if (pr->lexer->error)
		return (0);
	node = snode(syn_id);
	if (!pr_token_set(pr, node, tk_id))
	{
		node_destroy(node);
		pr->lexer->error = SH_PR_UNEXTOKEN;
		printf("Unexpected token %s:%d expected:%d got:%d, I'm in mode %d\n", __FILE__, __LINE__, tk_id, pr->current.id, pr->lexer->id);
		return (0);
	}
	if (parent == NULL)
		node_destroy(node);
	else
		node_add_child(parent, node);
	return (1);
}
