/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/04 10:50:42 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/15 15:09:32 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* https://pubs.opengroup.org/onlinepubs/009656399/toc.pdf */

# include <stddef.h>

# ifdef SH_DEBUG
#  define sh_assert(test) sh_assert2(test, #test, __FILE__, __LINE__)
# else
#  define sh_assert sh_assert1
# endif

typedef struct s_readline	t_readline;
typedef struct s_input		t_input;
typedef struct s_minishell	t_minishell;
typedef struct s_token		t_token;
typedef struct s_tokenizer	t_tokenizer;
typedef struct s_parser		t_parser;
typedef struct s_snode		t_snode;

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
	sx_cmd_suffix
};

enum e_node_flag
{
	flag_and,
	flag_semi
};

typedef enum e_token_id		t_token_id;
typedef enum e_syntax_id	t_syntax_id;
typedef enum e_node_flag	t_node_flag;

struct s_readline
{
	int	warning_silencer;
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
	int		id;
	char	*str;
	size_t	len;
};

struct s_tokenizer
{
	t_input	*in;
	char	*str;
	size_t	len;
	int		ch;
	int		ch2;
	int		bslash;
	int		quote;
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
	struct s_snode	*parent;
	struct s_snode	**childs;
	t_token			*token;
	int				flags;
};

char		*sh_readline(t_readline *rl, const char *prompt);
int			sh_readchar(t_input *in);

void		*sh_safe_malloc(size_t size);
void		*sh_safe_realloc(void *ptr, size_t old_size, size_t new_size);

void		tk_create(t_tokenizer *tk, t_input *in);
void		tk_destroy(t_tokenizer *tk);
t_token_id	tk_run(t_tokenizer *tk);
int			tk_tokenize(t_tokenizer *tk, t_token *token);
void		tk_debug(void);
int			tk_quoted(t_tokenizer *tk);
void		tk_append(t_tokenizer *tk);
void		tk_readchar(t_tokenizer *tk);
void		tk_skip(t_tokenizer *tk);
t_token_id	tk_op(t_tokenizer *tk);
t_token_id	tk_word(t_tokenizer *tk);

t_snode		*pr_parse(t_parser *pr);
void		pr_debug(void);

void		sh_assert1(int test);
void		sh_assert2(int test, const char *str, const char *file, int line);

#endif
