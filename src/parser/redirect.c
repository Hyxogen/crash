/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirect.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/28 10:20:35 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/28 16:05:58 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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
	t_snode	*node;

	if (!pr->current_ret)
		return (0);
	node = snode(sx_io_redirect);
	pr_token(pr, node, sx_io_number, tk_ionumber);
	if (pr_io_file(pr, node))
	{
		node_add_child(parent, node);
		return (1);
	}
	else if (pr_io_here(pr, node))
	{
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}

int
	pr_redirect_list(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_cmd_suffix);
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
