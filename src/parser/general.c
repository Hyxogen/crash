/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   general.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:03:53 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/01 13:25:11 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "memory.h"
#include <stdlib.h>

void
	pr_init(t_parser *pr)
{
	pr->current = NULL;
	pr->next = NULL;
	pr->current_ret = 0;
	pr->next_ret = 0;
	pr->lexer = NULL;
	pr->here_docs = NULL;
}

int
	pr_next_token(t_parser *pr)
{
	pr_check_here(pr);
	pr->current = pr->next;
	pr->current_ret = pr->next_ret;
	pr->next = 0;
	pr->next_ret = 0;
	if (!pr->current || pr->current->id != tk_newline)
	{
		pr->next = sh_safe_malloc(sizeof(*pr->next));
		pr->next_ret = lex_lex(pr->lexer, pr->next);
		if (pr->next_ret)
			pr_convert_io_number(pr, pr->next);
		else
		{
			token_destroy(pr->next);
			free(pr->next);
			pr->next_ret = 0;
			pr->next = NULL;
			return (pr->current_ret);
		}
		if (!pr->current)
			pr_next_token(pr);
	}
	return (pr->current_ret);
}

int
	pr_token_set(t_parser *pr, t_snode *node, t_token_id tk_id)
{
	if (pr->current_ret == 0 || pr->current->id != tk_id)
		return (0);
	node->token = pr->current;
	pr_next_token(pr);
	return (1);
}

int
	pr_token(t_parser *pr, t_snode *parent,
		t_syntax_id syn_id, t_token_id tk_id)
{
	t_snode	*node;

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