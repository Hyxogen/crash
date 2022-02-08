/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/04 10:50:42 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/08 09:11:53 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stddef.h>

typedef struct s_readline	t_readline;
typedef struct s_input		t_input;
typedef struct s_minishell	t_minishell;
typedef struct s_token		t_token;
typedef struct s_tokenizer	t_tokenizer;
typedef struct s_parser		t_parser;
typedef struct s_snode		t_snode;
typedef enum e_token_type	t_token_type;
typedef enum e_token_id		t_token_id;
typedef enum e_syntax_id	t_syntax_id;

enum e_token_type
{
	word,
	oper,
};

enum e_token_id
{
	token_id_null = -1,
	word,
	assignment_word,
	name,
	newline,
	io_number,
	op_and_if,
	op_or_if,
	op_dsemi,
	op_dless,
	op_dgreat,
	op_lessand,
	op_greatand,
	op_lessgreat,
	op_dlessdash,
	op_clobber,
	kw_if,
	kw_then,
	kw_else,
	kw_elif,
	kw_fi,
	kw_do,
	kw_done,
	kw_case,
	kw_esac,
	kw_while,
	kw_until,
	kw_for,
	lbrace,
	rbrace,
	bang,
	kw_in,
	op_and,
	op_pipe,
	op_lparen,
	op_rparen,
	op_less,
	op_great,
	op_semi
};

enum e_syntax_id
{
	sx_none,
	sx_and,
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
	sx_bang
};

struct s_readline
{
};

struct s_input
{
	t_readline	rl;
	char		*line;
	size_t		index;
	int			more;
};

struct s_token
{
	t_token_type	type;
	t_token_id		id;
	char			*str;
};

struct s_tokenizer
{
	t_token	*tok;
	t_input	*in;
	int		esc_ch;
	int		ch;
};

struct s_parser
{
	t_token		*current;
	int			current_ret;
	t_snode		*syntax_tree;
	t_snode		*current_node;
	t_tokenizer	*tokenizer;
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
	struct s_snode	*root;
	struct s_snode	**childs;
};

char	*sh_readline(t_readline *rl, const char *prompt);
int		sh_readchar(t_input *in);

void		*sh_safe_malloc(size_t size);
void		*sh_reallloc(void *ptr, size_t old_size, size_t new_size);

int			tk_readchar_raw(t_tokenizer *tk);
int			tk_readchar(t_tokenizer *tk);
void		tk_advance(t_tokenizer *tk);
int			tk_assume(t_tokenizer *tk, char ch);
t_token_id	tk_op(t_tokenizer *tk);
int			tk_tokenize(t_tokenizer *tk, t_token *token);

void		sh_assert(int condition);

#endif