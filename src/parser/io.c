/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   io.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:21:00 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/28 10:37:00 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void
	pr_process_here(void *data, void *param)
{
	t_parser	*pr;
	t_snode		*node;

	pr = param;
	node = data;
	/* TODO: unquote */
	lex_here(pr->lexer, pr->current, node->token->str, node->flags);
}

int
	pr_check_here(t_parser *pr)
{
	if (pr->current_ret && pr->current->id == tk_newline && ft_lstsize(pr->here_docs))
	{
		ft_lstforeach(pr->here_docs, pr_process_here, pr);
	}
	return (1);
}

int
	pr_io_file(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_io_file);
	if (pr_token(pr, node, sx_less, op_less)
		|| pr_token(pr, node, sx_lessand, op_lessand)
		|| pr_token(pr, node, sx_great, op_great)
		|| pr_token(pr, node, sx_greatand, op_greatand)
		|| pr_token(pr, node, sx_dgreat, op_dgreat)
		|| pr_token(pr, node, sx_lessgreat, op_lessgreat)
		|| pr_token(pr, node, sx_clobber, op_clobber))
	{
		if (pr_token(pr, node, sx_filename, tk_word))
		{
			node_add_child(parent, node);
			return (1);
		}
	}
	node_destroy(node);
	return (0);
}

int
	pr_io_here(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_io_here);
	if (pr_token(pr, NULL, sx_none, op_dless))
	{
		if (pr_token(pr, node, sx_word, tk_word))
		{
			ft_lstadd_back(&pr->here_docs, ft_lstnew(node->childs[0]));
			node_add_child(parent, node);
			pr_check_here(pr);
			return (1);
		}
	}
	else if (pr_token(pr, NULL, sx_none, op_dlessdash)
		&& pr_token(pr, node, sx_word, tk_word))
	{
		node->childs[0]->flags |= flag_trim;
		ft_lstadd_back(&pr->here_docs, ft_lstnew(node->childs[0]));
		node_add_child(parent, node);
		pr_check_here(pr);
		return (1);
	}
	node_destroy(node);
	return (0);
}
