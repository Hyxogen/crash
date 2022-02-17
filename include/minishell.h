/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/04 10:50:42 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/17 15:14:04 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* https://pubs.opengroup.org/onlinepubs/009656399/toc.pdf */

# include <stddef.h>
# include "lexer.h"

# ifdef SH_DEBUG
#  define sh_assert(test) sh_assert2(test, #test, __FILE__, __LINE__)
# else
#  define sh_assert sh_assert1
# endif

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
	sx_function_name
};

enum e_node_flag
{
	flag_and = 1,
	flag_semi = 1 << 1,
	flag_bang = 1 << 2,
	flag_and_if = 1 << 3,
	flag_or_if = 1 << 4
};

typedef enum e_syntax_id	t_syntax_id;
typedef enum e_node_flag	t_node_flag;
typedef struct s_minishell	t_minishell;
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

struct s_minishell
{
	t_input		in;
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

char		*sh_readline(t_readline *rl, const char *prompt);
int			sh_readchar(t_input *in);

void		*sh_safe_malloc(size_t size);
void		*sh_safe_realloc(void *ptr, size_t old_size, size_t new_size);

void		pr_convert_reserved(t_parser *pr, t_token *token);
int			pr_convert_keyword(t_parser *pr, t_token *token, t_token_id id);
int			pr_convert_name(t_parser *pr, t_token *token);
int			pr_convert_ass(t_parser *pr, t_token *token, int first);

t_snode		*pr_parse(t_parser *pr);
void		pr_debug(void);

void		sh_assert1(int test);
void		sh_assert2(int test, const char *str, const char *file, int line);

#endif
