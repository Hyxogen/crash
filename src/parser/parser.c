/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/07 11:35:51 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/16 16:13:43 by dmeijer       ########   odam.nl         */
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
int		pr_compound_list(t_parser *pr, t_snode *parent);

/*TEMPORARY*/
int
	pr_convert_io_number(t_parser *pr, t_token *token) {
	(void) pr;
	(void) token;
	return (0);
}

t_snode
	*node_init(t_snode *node, t_syntax_id syn_id)
{
	node->type = syn_id;
	node->parent = NULL;
	node->childs = NULL;
	node->childs_capacity = 0;
	node->childs_size = 0;
	node->token = NULL;
	node->flags = 0;
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
	pr->current_ret = lexer_lex(pr->lexer, pr->current);
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
	pr_io_file(t_parser *pr, t_snode *parent)
{
	t_snode *node;

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
	(void) pr;
	(void) parent;
	return (0);
}

int
	pr_io_redirect(t_parser *pr, t_snode *parent)
{
	t_snode	*node;
	t_token	cpy;

	if (!pr->current_ret)
		return (0);
	node = snode(sx_io_redirect);
	cpy = *pr->current;
	if (pr_convert_io_number(pr, &cpy))
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
	t_snode *node;

	node = snode(sx_cmd_suffix);
	while (!pr_io_redirect(pr, node)
		&& pr_token(pr, node, sx_word, tk_word))
		continue;
	if (node->childs_size != 0)
	{
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);	
}

int
	pr_simple_cmd(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = node_create();
	node_init(node, sx_simple_cmd);
	if (pr_cmd_prefix(pr, node))
		pr_token(pr, node, sx_cmd_word, tk_word);
	else if (!pr_token(pr, node, sx_cmd_word, tk_word))
	{
		node_destroy(node);
		return (0);
	}
	pr_cmd_suffix(pr, node);
	node_add_child(parent, node);
	return (1);
}

int
	pr_redirect_list(t_parser *pr, t_snode *parent)
{
	(void) pr;
	(void) parent;
	return (0);
}

int
	pr_subshell(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_subshell);
	if (pr_token(pr, NULL, sx_none, op_lparen))
	{
		if (pr_compound_list(pr, node))
		{
			if (pr_token(pr, NULL, sx_none, op_rparen))
			{
				node_add_child(parent, node);
				return (1);
			}
		}
	}
	node_destroy(node);
	return (0);
}

int
	pr_brace_group(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_brace_group);
	if (pr_token(pr, NULL, sx_none, kw_lbrace))
	{
		if (pr_compound_list(pr, node))
		{
			if (pr_token(pr, NULL, sx_none, kw_rbrace))
			{
				node_add_child(parent, node);
				return (1);
			}
		}
	}
	node_destroy(node);
	return (0);
}

int
	pr_compound_cmd(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_compound_cmd);
	if (pr_brace_group(pr, node) || pr_subshell(pr, node))
	{
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}

int
	pr_func_def(t_parser *pr, t_snode *parent)
{
	(void) pr;
	(void) parent;
	return (0);
}

int
	pr_cmd(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = node_create();
	node_init(node, sx_cmd);
	if (pr_func_def(pr, node))
		;
	else if (pr_compound_cmd(pr, node))
		pr_redirect_list(pr, node);
	else if (pr_simple_cmd(pr, node))
		;
	else
	{
		node_destroy(node);
		return (0);
	}
	node_add_child(parent, node);
	return (1);
}

int
	pr_pipe_sequence(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_pipe_sequence);
	if (pr_cmd(pr, node))
	{
		if (pr_token(pr, NULL, sx_pipe, op_pipe))
		{
			while (pr_token(pr, NULL, sx_newline, tk_newline))
				continue ;
			if (!pr_pipe_sequence(pr, node))
			{
				node_destroy(node);
				return (0);
			}
		}
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}

int
	pr_pipeline(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_pipeline);
	if (pr_token(pr, NULL, sx_bang, kw_bang))
		node->flags |= flag_bang;
	if (!pr_pipe_sequence(pr, node))
	{
		node_destroy(node);
		return (0);
	}
	node_add_child(parent, node);
	return (1);
}

/*Todo test if something like "echo Hallo && || cmd" doesn't work*/
int
	pr_and_or(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_and_or);
	if (pr_pipeline(pr, node))
	{
		node->flags |= flag_and_if * pr_token(pr, NULL, sx_and_if, op_andif)
			+ flag_or_if * pr_token(pr, NULL, sx_or_if, op_orif);
		if (!(node->flags & flag_or_if) != !(node->flags & flag_and_if))
		{
			while (pr_token(pr, NULL, sx_newline, tk_newline))
				continue ;
			if (!pr_and_or(pr, node))
			{
				node_destroy(node);
				return (0);
			}
		}
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}

int
	pr_list(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_list);
	if (pr_and_or(pr, node))
	{
		if (pr_token(pr, NULL, sx_and, op_and))
			node->flags |= flag_and;
		else if (pr_token(pr, NULL, sx_semicolon, op_semi))
			node->flags |= flag_semi;
		pr_list(pr, node);
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}

int
	pr_complete_cmd(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_complete_cmd);
	if (pr_list(pr, node))
	{
		while (pr_token(pr, NULL, sx_newline, tk_newline))
			continue ;
		if (!pr->current_ret)
		{
			node_add_child(parent, node);
			return (1);
		}
	}
	node_destroy(node);
	return (0);
}

int
	pr_term(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_term);
	if (pr_and_or(pr, node))
	{
		if (pr_token(pr, NULL, sx_and, op_and))
			node->flags |= flag_and;
		else if (pr_token(pr, NULL, sx_semicolon, op_semi))
			node->flags |= flag_semi;
		if (node->flags)
		{
			while (pr_token(pr, NULL, sx_newline, tk_newline))
				continue ;
			pr_term(pr, node);
		}
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}

int
	pr_compound_list(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_compound_list);
	while (pr_token(pr, NULL, sx_newline, tk_newline))
		continue ;
	if (pr_term(pr, node))
	{
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}
