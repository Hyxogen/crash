/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   general.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:03:53 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/24 13:41:00 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "memory.h"
#include <stdlib.h>

void
	pr_init(t_parser *pr)
{
	pr->current.id = tk_invalid;
	pr->next.id = tk_invalid;
	pr->lexer = NULL;
	pr->here_docs = NULL;
}

void
	pr_read_token(t_parser *pr, t_token *token)
{
	int	ret;

	ret = lex_lex(pr->lexer, token);
	if (ret == 1)
		pr_convert_io_number(pr, token);
	else if (ret == -1)
		pr->lexer->error = SH_PR_UNEXEOF;
	else
		token->id = tk_null;
}

int
	pr_next_token(t_parser *pr)
{
	token_move(&pr->current, &pr->next);
	if (pr->current.id == tk_invalid)
		pr_read_token(pr, &pr->current);
	if (pr->current.id != tk_newline
		&& pr->current.id != tk_null)
		pr_read_token(pr, &pr->next);
	pr_check_here(pr);
	return (pr->current.id != tk_invalid);
}

int
	pr_token_set(t_parser *pr, t_snode *node, t_token_id tk_id)
{
	if (pr->current.id != tk_id)
		return (0);
	token_move(&node->token, &pr->current);
	pr_next_token(pr);
	if (pr->lexer->error)
		return (0);
	return (1);
}

int
	pr_token(t_parser *pr, t_snode *parent,
		t_syntax_id syn_id, t_token_id tk_id)
{
	t_snode	*node;

	if (pr->lexer->error)
		return (0);
	node = snode(syn_id);
	if (!pr_token_set(pr, node, tk_id))
	{
		node_destroy(node);
		return (0);
	}
	if (parent == NULL)
		node_destroy(node);
	else
		node_add_child(parent, node);
	return (1);
}
