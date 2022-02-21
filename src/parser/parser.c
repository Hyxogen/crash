/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/07 11:35:51 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/21 14:24:48 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include <stdlib.h>
#include <libft.h>

#define SH_DEF_CHILD_SIZE 100

void	node_destroy(t_snode *node);
int		pr_compound_list(t_parser *pr, t_snode *parent);
int		pr_function_def(t_parser *pr, t_snode *parent);
int		pr_case_clause(t_parser *pr, t_snode *parent);

int
	pr_convert_io_number(t_parser *pr, t_token *token)
{
	size_t	i;

	if (pr->lexer->cur != '<' && pr->lexer->cur != '>')
		return (0);
	i = 0;
	while (i < token->length)
	{
		if (!ft_isdigit(token->string[i]))
			return (0);
		i += 1;
	}
	token->id = tk_ionumber;
	return (1);
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
	if (!pr->next_ret)
	{
		pr->next = sh_safe_malloc(sizeof(t_token));
		pr->next_ret = lexer_lex(pr->lexer, pr->next);
		pr_convert_io_number(pr, pr->next);
	}
	pr->current = pr->next;
	pr->current_ret = pr->next_ret;
	if (pr->current->id != tk_newline && pr->current_ret)
	{
		pr->next = sh_safe_malloc(sizeof(t_token));
		pr->next_ret = lexer_lex(pr->lexer, pr->next);
		pr_convert_io_number(pr, pr->next);
	}
	else
		pr->next_ret = 0;
	return (pr->current_ret);
}

int
	pr_token(t_parser *pr, t_snode *parent,
		t_syntax_id syn_id, t_token_id tk_id)
{
	t_snode *node;

	if (pr->current_ret == 0 || pr->current->id != tk_id)
		return (0);
	node = snode(syn_id);
	node->token = pr->current;
	pr_next_token(pr);
	if (!parent)
	{
		node_destroy(node);
		return (1);
	}
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
	t_snode *node;

	node = snode(sx_cmd_prefix);
	while (pr_io_redirect(pr, node)
		|| (pr_convert_ass(pr, pr->current, 1)
			&& pr_token(pr, node, sx_assword, tk_assword)))
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
	pr_cmd_suffix(t_parser *pr, t_snode *parent)
{
	t_snode *node;

	node = snode(sx_cmd_suffix);
	while (pr_io_redirect(pr, node)
		|| pr_token(pr, node, sx_word, tk_word))
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
	t_snode *node;

	node = snode(sx_cmd_suffix);
	while (pr_io_redirect(pr, node))
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
	pr_do_group(t_parser *pr, t_snode *parent)
{
	t_snode *node;

	node = snode(sx_do_group);
	pr_convert_reserved(pr, pr->current);
	if (pr_token(pr, NULL, sx_none, kw_do))
	{
		if (pr_compound_list(pr, node))
		{
			pr_convert_reserved(pr, pr->current);
			if (pr_token(pr, NULL, sx_none, kw_done))
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
	pr_while_clause(t_parser *pr, t_snode *parent)
{
	t_snode *node;

	node = snode(sx_while_clause);
	if (pr_token(pr, NULL, sx_none, kw_while))
	{
		if (pr_compound_list(pr, node))
		{
			if (pr_do_group(pr, node))
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
	pr_until_clause(t_parser *pr, t_snode *parent)
{
	t_snode *node;

	node = snode(sx_until_clause);
	if (pr_token(pr, NULL, sx_none, kw_until))
	{
		if (pr_compound_list(pr, node))
		{
			if (pr_do_group(pr, node))
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
	pr_condition(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_condition);
	if (pr_compound_list(pr, node))
	{
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}

int
	pr_elif_part(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_elif_part);
	if (pr_token(pr, NULL, sx_none, kw_elif)
		&& pr_condition(pr, node) && pr_token(pr, NULL, sx_none, kw_then)
		&& pr_compound_list(pr, node))
	{
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}

int
	pr_else_part(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_else_part);
	if (pr_token(pr, NULL, sx_none, kw_else)
		&& pr_compound_list(pr, node))
	{
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}

int
	pr_if_clause(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_if_clause);
	if (pr_token(pr, NULL, sx_none, kw_if)
		&& pr_condition(pr, node) && pr_token(pr, NULL, sx_none, kw_then)
		&& pr_compound_list(pr, node))
	{
		if (pr_token(pr, NULL, sx_none, kw_fi))
		{
			node_add_child(parent, node);
			return (1);
		}
		while (pr_elif_part(pr, node))
			continue ;
		pr_else_part(pr, node);
		if (pr_token(pr, NULL, sx_none, kw_fi))
		{
			node_add_child(parent, node);
			return (1);
		}
	}
	node_destroy(node);
	return (0);
}

int
	pr_wordlist(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_wordlist);
	while (pr_token(pr, node, sx_word, tk_word))
		continue ;
	if (node->childs_size != 0)
	{
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}

int
	pr_sequential_sep(t_parser *pr, t_snode *parent)
{
	(void) parent;
	if (!pr_token(pr, NULL, sx_none, op_semi)
		&& !pr_token(pr, NULL, sx_none, tk_newline))
		return (0);
	while (pr_token(pr, NULL, sx_newline, tk_newline))
		continue ;
	return (1);
}

int
	pr_for_clause(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_for_clause);
	if (pr_token(pr, NULL, sx_none, kw_for)
		&& pr_convert_name(pr, pr->current))
	{
		pr_token(pr, node, sx_for_name, tk_name);
		while (pr_token(pr, NULL, sx_newline, tk_newline))
			continue ;
		if (pr_convert_reserved(pr, pr->current)
			&& pr_token(pr, NULL, sx_none, kw_in)
			&& pr_wordlist(pr, node) && pr_sequential_sep(pr, node)
			&& pr_convert_reserved(pr, pr->current) && pr_do_group(pr, node))
		{
			node_add_child(parent, node);
			return (1);
		}
		else if (pr_do_group(pr, node))
		{
			node_add_child(parent, node);
			return (1);
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
	if (!pr->current_ret)
		return (0);
	pr_convert_reserved(pr, pr->current);
	if (pr_brace_group(pr, node)
		|| pr_subshell(pr, node)
		|| pr_while_clause(pr, node)
		|| pr_until_clause(pr, node)
		|| pr_if_clause(pr, node)
		|| pr_for_clause(pr, node)
		|| pr_case_clause(pr, node))
	{
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}

int
	pr_cmd(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = node_create();
	node_init(node, sx_cmd);
	if (pr_function_def(pr, node))
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
		if (pr_token(pr, NULL, sx_and_if, op_andif))
			node->flags |= flag_and_if;
		else if (pr_token(pr, NULL, sx_or_if, op_orif))
			node->flags |= flag_or_if;
		if (node->flags)
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

	if (!pr->current_ret)
		return (0);
	node = snode(sx_complete_cmd);
	if (pr_list(pr, node))
	{
		if (!pr->next_ret)
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
		while (pr_token(pr, NULL, sx_newline, tk_newline))
			node->flags |= flag_newline;
		if (node->flags)
			pr_term(pr, node);
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

int
	pr_convert_func_def(t_parser *pr)
{
	if (pr->next->id == op_lparen)
		if (pr_convert_name(pr, pr->current))
			return (1);
	return (0);
}

int
	pr_function_body(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_function_body);
	if (pr_compound_cmd(pr, node))
	{
		pr_redirect_list(pr, node);
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}

int
	pr_function_def(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	if (!pr_convert_func_def(pr))
		return (0);
	node = snode(sx_function_def);
	pr_token(pr, node, sx_function_name, tk_name);
	pr_token(pr, NULL, sx_none, op_lparen);
	if (pr_token(pr, NULL, sx_none, op_rparen))
	{
		while (pr_token(pr, NULL, sx_none, tk_newline))
			continue ;
		if (pr_function_body(pr, node))
		{
			node_add_child(parent, node);
			return (1);
		}
	}
	node_destroy(node);
	return (0);
}

int
	pr_pattern(t_parser *pr, t_snode *parent)
{
	if (!pr->current_ret)
		return (0);
	if (pr_convert_keyword(pr, pr->current, kw_esac))
		return (0);
	pr_token(pr, parent, sx_pattern, tk_word);
	return (1);
}

int
	pr_case_item(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_case_item);
	pr_token(pr, NULL, sx_none, op_lparen);
	if (pr_pattern(pr, node)
		&& pr_token(pr, NULL, sx_none, op_rparen))
	{
		if (!pr_compound_list(pr, node))
		{
			while (pr_token(pr, NULL, sx_none, tk_newline))
				continue ;
		}
		if (pr_token(pr, NULL, sx_none, op_dsemi))
		{
			while (pr_token(pr, NULL, sx_none, tk_newline))
				continue ;
			node_add_child(parent, node);
			return (1);
		}
	}
	node_destroy(node);
	return (0);
}

int
	pr_case_list(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	node = snode(sx_case_list);
	if (pr_case_item(pr, node))
	{
		pr_case_list(pr, node);
		node_add_child(parent, node);
		return (1);
	}
	node_destroy(node);
	return (0);
}

int
	pr_case_clause(t_parser *pr, t_snode *parent)
{
	t_snode	*node;

	if (!pr_token(pr, NULL, sx_none, kw_case))
		return (0);
	node = snode(sx_case_clause);
	if (pr_token(pr, node, sx_word, tk_word))
	{
		while (pr_token(pr, NULL, sx_none, tk_newline))
			continue ;
		if (pr_convert_keyword(pr, pr->current, kw_in)
			&& pr_token(pr, NULL, sx_none, kw_in))
		{
			while (pr_token(pr, NULL, sx_none, tk_newline))
				continue ;
			pr_case_list(pr, node);
			if (pr_token(pr, NULL, sx_none, kw_esac))
			{
				node_add_child(parent, node);
				return (1);
			}
		}
	}
	node_destroy(node);
	return (0);
}
