/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/07 11:35:51 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/15 13:05:45 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <libft.h>

/*
echo "echo Hallo" | bash > test; echo "Running stuff"

[list]
WORD WORD pipe_op WORD redirect_op WORD separator_op WORD WORD

[list]
[and_or]																	[separator_op]
[pipeline]																	[separator_op]	[and_or]
[pipe_sequence]																[separator_op]	[pipeline]
[pipe_sequence] [pipe_op] [linebreak] [command]								[separator_op]	[pipe_sequence]
[command] [pipe_op] [linebreak] [command]									[separator_op]	[command]
[simple_command] [pipe_op] [linebreak] [simple_command]						[separator_op]	[simple_command]
[cmd_name] [cmd_suffix] [pipe_op] [linebreak] [cmd_name] [cmd_suffix]		[separator_op]	[cmd_name] [cmd_suffix]
[word] [word] [pipe_op] [line_break] [word] [io_redirect]					[separator_op]	[word] [word]
[word] [word] [pipe_op] [line_break] [word] [io_file]						[separator_op]	[word] [word]
[word] [word] [pipe_op] [line_break] [word] [red_right] [filename]			[separator_op]	[word] [word]


and_or separator_op and_or (separator_op and_or (separator_op and_or))

*/

#define SH_DEF_CHILD_SIZE 100

void	node_destroy(t_snode *node);

t_snode
	*node_init(t_snode *node, t_syntax_id syn_id)
{
	node->type = syn_id;
	node->parent = NULL;
	node->childs = NULL;
	node->childs_capacity = 0;
	node->childs_size = 0;
	node->token = NULL;
	return (node);
}

t_snode
	*node_create(void)
{
	return (sh_safe_malloc(sizeof(t_snode)));
}

t_snode
	*snode(t_syntax_id syn_id)
{
	t_snode	*node;

	node = node_create();
	node_init(node, syn_id);
	return (node);
}

/*shrinking is undefined*/
void
	node_resize_childs(t_snode *node, size_t newsize)
{
	node->childs = sh_safe_realloc(
			node->childs,
			sizeof(t_snode*) * node->childs_capacity,
			sizeof(t_snode) * newsize);
	node->childs_capacity = newsize;
}

void
	node_add_child(t_snode *node, t_snode *child)
{
	child->parent = node;
	if (node->childs_size == node->childs_capacity)
		node_resize_childs(node, node->childs_capacity + SH_DEF_CHILD_SIZE);
	node->childs[node->childs_size] = child;
	node->childs_size++;
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
	free(node);
}

int
	pr_next_token(t_parser *pr)
{
	free(pr->current);
	pr->current = sh_safe_malloc(sizeof(t_token));
	sh_assert(pr->current != NULL);
	pr->current_ret = tk_tokenize(pr->tokenizer, pr->current);
	return (pr->current_ret);
}

int
	pr_token(t_parser *pr, t_snode *parent, t_syntax_id syn_id, t_token_id tk_id)
{
	t_snode *node;

	if (pr->current_ret == 0 || pr->current->id != tk_id)
		return (0);
	pr_next_token(pr);
	if (!parent)
		return (1);
	node = node_create();
	node_init(node, syn_id);
	node->token = pr->current;
	node_add_child(parent, node);
	return (1);
}

int
	pr_seperator_op(t_parser *pr, t_snode *parent)
{
	if (pr_token(pr, NULL, sx_and, op_and))
	{
		parent->flags |= flag_and;
		return (1);
	}
	if (pr_token(pr, NULL, sx_semicolon, op_semi))
	{
		parent->flags |= flag_semi;
		return (1);
	}
	return (0);
}

int
	pr_cmd_prefix(t_parser *pr, t_snode *parent)
{
	(void) pr;
	(void) parent;
	return (0);	
}

int
	pr_cmd_suffix(t_parser *pr, t_snode *parent)
{
	(void) pr;
	(void) parent;
	return (0);	
}

int
	pr_simple_cmd(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = node_create();
	node_init(node, sx_simple_cmd);
	if (pr_cmd_prefix(pr, node))
		pr_token(pr, node, sx_cmd_word, word);
	else if (!pr_token(pr, node, sx_cmd_word, word))
	{
		node_destroy(node);
		return (0);
	}
	pr_cmd_suffix(pr, parent);
	node_add_child(parent, node);
	return (1);
}

int
	pr_complete_cmd(t_parser *pr)
{
	t_snode	*node;

	node = node_create();
	node_init(node, sx_complete_cmd);
	if (!pr_simple_cmd(pr, node))
	{
		node_destroy(node);
		return (0);
	}
	return (1);
}
