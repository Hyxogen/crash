/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/07 11:35:51 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/07 16:32:54 by csteenvo      ########   odam.nl         */
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

void
	pr_get_next_token(t_parser *pr)
{
	free(pr->current);
	pr->current = malloc(sizeof(t_token));
	sh_assert(pr->current);
	pr->current_ret = tk_tokenize(pr->tokenizer, pr->current);
}

t_snode
	*pr_newline_list(t_parser *pr)
{
	t_snode *newline_list_node;

	newline_list_node = malloc(sizeof(*newline_list_node));
	if (!pr->current_ret)
		return (NULL);
}

t_snode
	*pr_linebreak(t_parser *pr)
{
	t_snode *linebreak_node;

	linebreak_node = malloc(sizeof(*linebreak_node));
	linebreak_node->type = nt_linebreak;
	if (!pr->current_ret)
		return (linebreak_node);
	linebreak_node->childs[0] = pr_newline_list(pr);
	if (!linebreak_node->childs[0])
		return (NULL);
	return (linebreak_node);
}

t_snode
	*pr_and_or(t_parser *pr)
{
	t_snode	*and_or_node;
	t_snode	*temp_node;

	and_or_node = malloc(sizeof(*and_or_node));
	and_or_node->type = nt_and_or;
	pr_get_next_token(pr);
	temp_node = pr_pipeline(pr);
	and_or_node->childs[0] = temp_node;
	temp_node = pr_linebreak(pr);
	if (!temp_node)
		return (and_or_node);
	if (pr->current != op_and_if && pr->current != op_or_if)
		return (NULL);
	and_or_node->childs[1] = temp_node;
	and_or_node->childs[2] = pr_and_or(pr);
	if (!and_or_node->childs[2])
		return (NULL);
	return (and_or_node);
}

t_snode
	*pr_list(t_parser *pr)
{
	t_snode	*list_node;
	t_snode	*and_or_node;
	t_token	next_token;
	size_t	index;

	list_node = malloc(sizeof(*list_node));
	tk_tokenize(pr->tokenizer, &next_token);
	pr->current = &next_token;
	index = 0;
	while (1)
	{
		and_or_node = pr_and_or(pr);
		list_node->childs[index] = and_or_node;
		if (pr->current->id != op_and && pr->current->id != op_semi)
			return (list_node);
		tk_tokenize(pr->tokenizer, &next_token);
		pr->current = &next_token;
	}
}

t_snode
	*pr_complete_command(t_parser *pr)
{
	t_snode	*complete_command_node;
	t_snode	*list_node;
	t_token next_token;
	size_t	index;

	complete_command_node = malloc(sizeof(*complete_command_node));
	index = 0;
	tk_tokenize(pr->tokenizer, &next_token);
	pr->current = &next_token;
	while (1)
	
		list_node = pr_list(pr);
		complete_command_node->childs[index] = list_node;
		if (pr->current->id != op_and && pr->current->id != op_semi && pr->current->id != newline)
			return (complete_command_node);
		tk_tokenize(pr->tokenizer, &next_token);
		pr->current = &next_token;
		index++;
	}
}