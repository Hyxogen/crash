/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/21 11:30:58 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/21 11:33:17 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

enum e_syntax_id
{
	sx_none,
	sx_and,
	sx_cmd_word,
	sx_semicolon,
	sx_separator_op,
	sx_separator,
	sx_and_if,
	sx_or_if,
	sx_pipe,
	sx_newline,
	sx_newline_list,
	sx_word,
	sx_assword,
	sx_simple_cmd,
	sx_pipe_sequence,
	sx_pipeline,
	sx_list,
	sx_complete_cmd,
	sx_and_or,
	sx_linebreak,
	sx_bang,
	sx_io_file,
	sx_less,
	sx_lessand,
	sx_great,
	sx_greatand,
	sx_dgreat,
	sx_lessgreat,
	sx_clobber,
	sx_filename,
	sx_io_redirect,
	sx_io_number,
	sx_cmd_suffix,
	sx_cmd_prefix,
	sx_cmd,
	sx_term,
	sx_compound_list,
	sx_subshell,
	sx_brace_group,
	sx_compound_cmd,
	sx_while_clause,
	sx_until_clause,
	sx_do_group,
	sx_if_clause,
	sx_else_part,
	sx_function_def,
	sx_function_body,
	sx_function_name,
	sx_case_clause,
	sx_case_list,
	sx_case_item,
	sx_pattern,
	sx_for_clause,
	sx_for_name,
	sx_wordlist,
	sx_condition,
	sx_elif_part
};

enum e_node_flag
{
	flag_and = 1,
	flag_semi = 1 << 1,
	flag_bang = 1 << 2,
	flag_and_if = 1 << 3,
	flag_or_if = 1 << 4,
	flag_newline = 1 << 5
};

typedef enum e_syntax_id	t_syntax_id;
typedef enum e_node_flag	t_node_flag;
typedef struct s_parser		t_parser;
typedef struct s_snode		t_snode;

struct s_parser
{
	t_token		*current;
	t_token		*next;
	int			current_ret;
	int			next_ret;
	t_snode		*syntax_tree;
	t_snode		*current_node;
	t_lexer		*lexer;
};

struct s_snode
{
	t_syntax_id		type;
	size_t			childs_capacity;
	size_t			childs_size;
	struct s_snode	*parent;
	struct s_snode	**childs;
	t_token			*token;
	int				flags;
};

int		pr_convert_reserved(t_parser *pr, t_token *token);
int		pr_convert_keyword(t_parser *pr, t_token *token, t_token_id id);
int		pr_convert_name(t_parser *pr, t_token *token);
int		pr_convert_ass(t_parser *pr, t_token *token, int first);

t_snode	*pr_parse(t_parser *pr);

#endif
