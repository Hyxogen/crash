/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   io.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:21:00 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/24 11:39:29 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "memory.h"
#include <stdlib.h>

void
	pr_process_here(void *data, void *param)
{
	t_parser	*pr;
	t_snode		*node;
	char		*end;

	pr = param;
	node = data;
	end = ft_strdup(node->token.str);
	if (lex_unquote(end))
		node->flags |= flag_quote;
	lex_here(pr->lexer, &node->here_content, end, node->flags);
	free(end);
}

int
	pr_check_here(t_parser *pr)
{
	if (pr->current.id == tk_newline
		&& ft_lstsize(pr->here_docs))
	{
		ft_lstforeach(pr->here_docs, pr_process_here, pr);
		ft_lstclear(&pr->here_docs, sh_nop);
	}
	return (1);
}

int
	pr_io_file(t_parser *pr, t_snode *parent)
{
	if (pr_token(pr, parent, sx_less, op_less)
		|| pr_token(pr, parent, sx_lessand, op_lessand)
		|| pr_token(pr, parent, sx_great, op_great)
		|| pr_token(pr, parent, sx_greatand, op_greatand)
		|| pr_token(pr, parent, sx_dgreat, op_dgreat)
		|| pr_token(pr, parent, sx_lessgreat, op_lessgreat)
		|| pr_token(pr, parent, sx_clobber, op_clobber))
	{
		token_destroy(&parent->childs[parent->childs_size - 1]->token);
		parent->childs[parent->childs_size - 1]->token.id = tk_invalid;
		if (pr_error_token(pr, parent->childs[parent->childs_size - 1],
				sx_filename, tk_word))
			return (1);
	}
	return (0);
}

int
	pr_io_here(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_io_here);
	if (pr_token(pr, NULL, sx_none, op_dless)
		&& pr_error_token(pr, node, sx_word, tk_word))
	{
		ft_lstadd_back(&pr->here_docs, ft_lstnew(node->childs[0]));
		node_add_child(parent, node);
		pr_check_here(pr);
		return (1);
	}
	else if (pr_token(pr, NULL, sx_none, op_dlessdash)
		&& pr_error_token(pr, node, sx_word, tk_word))
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
