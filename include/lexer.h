#ifndef LEXER_H
# define LEXER_H

# include <stddef.h>
# include "input.h"

enum e_token_id
{
	tk_null,
	tk_word,
	tk_assword,
	tk_name,
	tk_newline,
	tk_ionumber,
	op_andif,
	op_orif,
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
	kw_lbrace,
	kw_rbrace,
	kw_bang,
	kw_in,
	op_and,
	op_pipe,
	op_lparen,
	op_rparen,
	op_less,
	op_great,
	op_semi
};

enum e_expansion_id
{
	xp_word,
	xp_parameter,
	xp_command,
	xp_arithmetic
};

typedef enum e_token_id			t_token_id;
typedef enum e_expansion_id		t_expansion_id;
typedef struct s_token			t_token;
typedef struct s_expansion		t_expansion;
typedef struct s_lexer			t_lexer;
typedef struct s_lexer_state	t_lexer_state;

struct s_expansion
{
	t_expansion_id	id;
	char			*string;
	size_t			length;
	int				quoted;
};

struct s_token
{
	t_token_id	id;
	char		*string;
	size_t		length;
	t_expansion	*xps;
	size_t		count;
};

struct s_lexer
{
	t_input			*in;
	t_token			*tok;
	int				cur;
	int				next;
	int				quote;
	int				bslash;
	int				btick;
	const char		*end;
	t_expansion_id	xp_id;
};

struct s_lexer_state
{
	int				quote;
	int				was_word;
	const char		*end;
	t_expansion_id	xp_id;
};

int			lexer_new(t_lexer *lex, t_input *in);
void		lexer_delete(t_lexer *lex);
int			token_new(t_token *tok);
int			token_start(t_token *tok);
void		token_delete(t_token *tok);

int			lexer_quoted(t_lexer *lex);
int			lexer_bquoted(t_lexer *lex);
void		lexer_read(t_lexer *lex, int xp_sep);
void		lexer_read_raw(t_lexer *lex);

t_token_id	lexer_op(t_lexer *lex);
int			lexer_expand_dollar(t_lexer *lex, t_expansion_id id);
int			lexer_expand_backtick(t_lexer *lex);
int			lexer_recurse(t_lexer *lex);

int			lexer_lex(t_lexer *lex, t_token *tok);
int			lexer_heredoc(t_lexer *lex, t_token *tok, const char *end);
void		lexer_debug(void);

#endif
