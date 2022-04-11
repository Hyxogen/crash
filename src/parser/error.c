/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   error.c                                          -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:23:11 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:23:11 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

#include <stdio.h>
#include <unistd.h>

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
		return (0);
	}
	if (parent == NULL)
		node_destroy(node);
	else
		node_add_child(parent, node);
	return (1);
}

int
	pr_error_convert_keyword(t_parser *pr, t_token *token, t_token_id id)
{
	size_t	len;

	(void) pr;
	if (token != NULL && token->id == id)
		return (1);
	if (token == NULL || token->id != tk_word)
		return (0);
	len = ft_strlen(pr_get_keywords()[id - kw_if]);
	if (ft_memcmp(pr_get_keywords()[id - kw_if], token->str, len + 1) == 0)
	{
		token->id = id;
		return (1);
	}
	pr->lexer->error = SH_PR_UNEXTOKEN;
	return (0);
}
