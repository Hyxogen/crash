/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/21 11:30:58 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/28 15:03:43 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "libft.h"
# include "lexer.h"

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
	sx_elif_part,
	sx_io_here,
	sx_complete_cmdlst
};

enum e_node_flag
{
	flag_and = 1,
	flag_semi = 1 << 1,
	flag_bang = 1 << 2,
	flag_and_if = 1 << 3,
	flag_or_if = 1 << 4,
	flag_newline = 1 << 5,
	flag_trim = 1 << 6,
	flag_quote = 1 << 7
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
	t_lexer		*lexer;
	t_list		*here_docs;
};

struct s_snode
{
	t_syntax_id		type;
	size_t			childs_capacity;
	size_t			childs_size;
	struct s_snode	*parent;
	struct s_snode	**childs;
	t_token			*token;
	t_token			*here_content;
	int				flags;
};

int		pr_convert_reserved(t_parser *pr, t_token *token);
int		pr_convert_keyword(t_parser *pr, t_token *token, t_token_id id);
int		pr_convert_name(t_parser *pr, t_token *token);
int		pr_convert_ass(t_parser *pr, t_token *token, int first);

t_snode	*pr_parse(t_parser *pr);
void	pr_init(t_parser *pr);
int		pr_next_token(t_parser *pr);
int		pr_complete_cmdlst(t_parser *pr, t_snode *parent);
int		pr_complete_cmd(t_parser *pr, t_snode *parent);
t_snode	*snode(t_syntax_id syn_id);
void	pr_destroy(t_parser *pr);


int		pr_and_or(t_parser *pr, t_snode *parent);
int		pr_case_item(t_parser *pr, t_snode *parent);
int		pr_case_list(t_parser *pr, t_snode *parent);
int		pr_case_clause(t_parser *pr, t_snode *parent);
int		pr_cmd_prefix(t_parser *pr, t_snode *parent);
int		pr_cmd_suffix(t_parser *pr, t_snode *parent);
int		pr_compound_cmd(t_parser *pr, t_snode *parent);
int		pr_cmd(t_parser *pr, t_snode *parent);
int		pr_complete_cmd(t_parser *pr, t_snode *parent);
int		pr_complete_cmdlst(t_parser *pr, t_snode *parent);
int		pr_simple_cmd(t_parser *pr, t_snode *parent);
int		pr_elif_part(t_parser *pr, t_snode *parent);
int		pr_else_part(t_parser *pr, t_snode *parent);
int		pr_if_clause(t_parser *pr, t_snode *parent);
int		pr_condition(t_parser *pr, t_snode *parent);
int		pr_convert_reserved(t_parser *pr, t_token *token);
int		pr_convert_keyword(t_parser *pr, t_token *token, t_token_id id);
int		pr_convert_name(t_parser *pr, t_token *token);
int		pr_convert_ass(t_parser *pr, t_token *token, int first);
int		pr_convert_func_def(t_parser *pr);
int		pr_function_body(t_parser *pr, t_snode *parent);
int		pr_function_def(t_parser *pr, t_snode *parent);
void	pr_init(t_parser *pr);
int		pr_next_token(t_parser *pr);
int		pr_token(t_parser *pr, t_snode *parent, t_syntax_id syn_id, t_token_id tk_id);
void	pr_process_here(void *data, void *param);
int		pr_check_here(t_parser *pr);
int		pr_io_file(t_parser *pr, t_snode *parent);
int		pr_io_here(t_parser *pr, t_snode *parent);
int		pr_compound_list(t_parser *pr, t_snode *parent);
int		pr_list(t_parser *pr, t_snode *parent);
int		pr_for_clause(t_parser *pr, t_snode *parent);
int		pr_while_clause(t_parser *pr, t_snode *parent);
int		pr_until_clause(t_parser *pr, t_snode *parent);
int		pr_do_group(t_parser *pr, t_snode *parent);
void	node_resize_childs(t_snode *node, size_t newsize);
t_snode	*node_init(t_snode *node, t_syntax_id syn_id);
t_snode	*snode(t_syntax_id syn_id);
void	node_add_child(t_snode *node, t_snode *child);
void	node_destroy_childs(t_snode *node);
void	node_destroy(t_snode *node);
int		pr_brace_group(t_parser *pr, t_snode *parent);
int		pr_wordlist(t_parser *pr, t_snode *parent);
int		pr_term(t_parser *pr, t_snode *parent);
int		pr_pattern(t_parser *pr, t_snode *parent);
int		pr_pipe_sequence(t_parser *pr, t_snode *parent);
int		pr_pipeline(t_parser *pr, t_snode *parent);
int		pr_convert_io_number(t_parser *pr, t_token *token);
int		pr_io_redirect(t_parser *pr, t_snode *parent);
int		pr_redirect_list(t_parser *pr, t_snode *parent);
int		pr_seperator_op(t_parser *pr, t_snode *parent);
int		pr_sequential_sep(t_parser *pr, t_snode *parent);
int		pr_subshell(t_parser *pr, t_snode *parent);
int		pr_bang(t_parser *pr, t_snode *parent);
#endif
