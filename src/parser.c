/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/07 11:35:51 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/10 11:10:33 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

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

/*TODO make parse functions like int pr_thing(t_parser *pr, t_snode **out) which returns whether or not the next token
should be retrieved*/

t_snode	*pr_pipeline(t_parser *pr);

t_snode
	*node_create_empty(void)
{
	t_snode	*node;

	node = sh_safe_malloc(sizeof(*node));
	node->type = sx_none;
	node->root = NULL;
	node->childs = NULL;
	node->childs_capacity = 0;
	node->childs_size = 0;
	return (node);
}

t_snode
	*node_create_id(t_syntax_id id)
{
	t_snode	*node;

	node = node_create_empty();
	node->type = id;
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
	child->root = node;
	if (node->childs_size == node->childs_capacity)
		node_resize_childs(node, node->childs_capacity + SH_DEF_CHILD_SIZE);
	node->childs[node->childs_size] = child;
	node->childs_size++;
}

void
	node_destroy(t_snode *node)
{
	free(node->childs);
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
	pr_expect_node(t_parser *pr, t_snode *root, t_snode *(*func)(t_parser*), int forward)
{
	t_snode	*expected_node;

	if (pr->current_ret == 0)
		return (0);
	expected_node = func(pr);
	if (!expected_node)
		return (0);
	node_add_child(root, expected_node);
	if (forward)
		pr_next_token(pr);
	return (1);
}

t_snode
	*pr_bang(t_parser *pr)
{
	if (pr->current->id != bang)
		return (NULL);
	return (node_create_id(sx_bang));
}

t_snode
	*pr_newline(t_parser *pr)
{
	if (pr->current->id != newline)
		return (NULL);
	return (node_create_id(sx_newline));
}

t_snode
	*pr_pipe(t_parser *pr)
{
	if (pr->current->id != op_pipe)
		return (NULL);
	return (node_create_id(sx_pipe));
}

t_snode
	*pr_and_if(t_parser *pr)
{
	if (pr->current->id != op_and_if)
		return (NULL);
	return (node_create_id(sx_and_if));
}

t_snode
	*pr_or_if(t_parser *pr)
{
	if (pr->current->id != op_or_if)
		return (NULL);
	return (node_create_id(sx_or_if));
}

t_snode
	*pr_newline_list(t_parser *pr)
{
	t_snode	*newline_list_node;

	newline_list_node = node_create_id(sx_newline_list);
	if (!pr_expect_node(pr, newline_list_node, pr_newline, 1))
		return (NULL);
	pr_expect_node(pr, newline_list_node, pr_newline_list, 1);
	return (newline_list_node);
}

t_snode
	*pr_linebreak(t_parser *pr)
{
	t_snode	*linebreak_node;

	linebreak_node = node_create_id(sx_linebreak);
	pr_expect_node(pr, linebreak_node, pr_newline_list, 1);
	return (linebreak_node);
}

/*TODO implement*/
t_snode
	*pr_cmd_suffix(t_parser *pr)
{
	(void)pr;
	return (NULL);
}

/*TODO apply rule 7a*/
t_snode
	*pr_cmd_name(t_parser *pr)
{
	if (pr->current->id != word)
		return (NULL);
	return (node_create_id(sx_cmd_name));
}

/*TODO apply rule 7b*/
t_snode
	*pr_cmd_word(t_parser *pr)
{
	if (pr->current->id != word)
		return (NULL);
	return (node_create_id(sx_cmd_word));
}

/*TODO implement*/
t_snode
	*pr_cmd_prefix(t_parser *pr)
{
	(void)pr;
	return (NULL);
}

t_snode
	*pr_and(t_parser *pr)
{
	if (pr->current->id != op_and)
		return (NULL);
	return (node_create_id(sx_and));
}

t_snode
	*pr_semicolon(t_parser *pr)
{
	if (pr->current->id != op_semi)
		return (NULL);
	return (node_create_id(sx_semicolon));
}

t_snode
	*pr_separator_op(t_parser *pr)
{
	t_snode	*separator_op_node;

	separator_op_node = node_create_id(sx_separator_op);
	if (!pr_expect_node(pr, separator_op_node, pr_and, 1)
		&& !pr_expect_node(pr, separator_op_node, pr_semicolon, 1))
		return (NULL);
	return (separator_op_node);
}

t_snode
	*pr_separator(t_parser *pr)
{
	t_snode	*separator_node;

	separator_node = node_create_id(sx_separator);
	if (pr_expect_node(pr, separator_node, pr_separator_op, 0))
	{
		if (!pr_expect_node(pr, separator_node, pr_linebreak, 1))
			return (NULL);
		return (separator_node);
	}
	if (!pr_expect_node(pr, separator_node, pr_newline_list, 0))
		return (NULL);
	return (separator_node);
}

t_snode
	*pr_simple_command(t_parser *pr)
{
	t_snode	*simple_command_node;

	simple_command_node = node_create_id(sx_simple_cmd);
	if (!pr_expect_node(pr, simple_command_node, pr_cmd_prefix, 0))
	{
		if (!pr_expect_node(pr, simple_command_node, pr_cmd_name, 1))
			return (NULL);
		pr_expect_node(pr, simple_command_node, pr_cmd_suffix, 1);
		return (simple_command_node);
	}
	if (!pr_expect_node(pr, simple_command_node, pr_cmd_word, 1))
		return (simple_command_node);
	pr_expect_node(pr, simple_command_node, pr_cmd_suffix, 0);
	return (simple_command_node);
}

/*TODO add compound command*/
t_snode
	*pr_command(t_parser *pr)
{
	return (pr_simple_command(pr));
}

t_snode
	*pr_pipe_sequence(t_parser *pr)
{
	t_snode	*pipe_sequence_node;

	pipe_sequence_node = node_create_id(sx_pipe_sequence);
	if (!pr_expect_node(pr, pipe_sequence_node, pr_command, 0))
		return (NULL);
	if (!pr_expect_node(pr, pipe_sequence_node, pr_linebreak, 0))
		return (pipe_sequence_node);
	if (!pr_expect_node(pr, pipe_sequence_node, pr_pipe, 1))
		return (NULL);
	if (!pr_expect_node(pr, pipe_sequence_node, pr_pipe_sequence, 0))
		return (NULL);
	return (pipe_sequence_node);
}

t_snode
	*pr_pipeline(t_parser *pr)
{
	t_snode	*pipeline_node;

	pipeline_node = node_create_id(sx_pipeline);
	pr_expect_node(pr, pipeline_node, pr_bang, 1);
	if (!pr_expect_node(pr, pipeline_node, pr_pipe_sequence, 0))
		return (NULL);
	return (pipeline_node);
}

t_snode
	*pr_and_or(t_parser *pr)
{
	t_snode	*and_or_node;

	and_or_node = node_create_id(sx_and_or);
	if (!pr_expect_node(pr, and_or_node, pr_pipeline, 0))
		return (NULL);
	if (!pr_expect_node(pr, and_or_node, pr_linebreak, 1))
		return (and_or_node);
	if (!pr_expect_node(pr, and_or_node, pr_and_if, 1)
		&& !pr_expect_node(pr, and_or_node, pr_or_if, 1))
		return (NULL);
	if (!pr_expect_node(pr, and_or_node, pr_and_or, 0))
		return (NULL);
	return (and_or_node);
}

t_snode
	*pr_list(t_parser *pr)
{
	t_snode	*list_node;

	list_node = node_create_id(sx_list);
	if (!pr_expect_node(pr, list_node, pr_and_or, 0))
		return (NULL);
	if (!pr_expect_node(pr, list_node, pr_separator, 0))
		return (list_node);
	if (!pr_expect_node(pr, list_node, pr_list, 0))
		return (NULL);
	return (list_node);
}

t_snode
	*pr_complete_cmd(t_parser *pr)
{
	t_snode	*complete_cmd_node;

	complete_cmd_node = node_create_id(sx_complete_cmd);
	if (!pr_expect_node(pr, complete_cmd_node, pr_list, 0))
		return (NULL);
	if (!pr_expect_node(pr, complete_cmd_node, pr_separator, 0))
		return (complete_cmd_node);
	return (complete_cmd_node);
}
