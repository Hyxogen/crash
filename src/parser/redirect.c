/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   redirect.c                                       -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:23:29 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:23:29 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

int
	pr_convert_io_number(t_parser *pr, t_token *token)
{
	size_t	i;

	if (pr->lexer->src->cur != '<' && pr->lexer->src->cur != '>')
		return (0);
	i = 0;
	while (i < token->len)
	{
		if (!ft_isdigit(token->str[i]))
			return (0);
		i += 1;
	}
	token->id = tk_ionumber;
	return (1);
}

int
	pr_io_redirect(t_parser *pr, t_snode *parent)
{
	t_token	token;

	if (pr->current.id == tk_invalid)
		return (0);
	token_init(&token); /* NOTE: Can prob be removed */
	if (pr->current.id == tk_ionumber)
	{
		token_move(&token, &pr->current);
		pr_next_token(pr);
	}
	if (pr_io_file(pr, parent)
		|| pr_io_here(pr, parent))
	{
		token_move(&parent->childs[parent->childs_size - 1]->token, &token);
		token_destroy(&token);
		return (1);
	}
	token_destroy(&token);
	return (0);
}

int
	pr_redirect_list(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_io_redirect_list);
	while (pr_io_redirect(pr, node))
		continue ;
	if (node->childs_size != 0)
	{
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}
