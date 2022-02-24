#ifndef LEXER_H
# define LEXER_H

# include <stddef.h>
# include "input.h"
# include "libft.h"

#define HERE_FLAG_TRIM 1
#define HERE_FLAG_QUOTE 2

struct s_snode;

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

enum e_lexer_id
{
	lx_normal,
	lx_command,
	lx_parameter,
	lx_arithmetic,
	lx_backtick
};

typedef enum e_token_id	t_token_id;
typedef enum e_lexer_id	t_lexer_id;
typedef struct s_tpart	t_tpart;
typedef struct s_token	t_token;
typedef struct s_lexer	t_lexer;
typedef struct s_source	t_source;

/* a token is made up of parts: */
/* - raw string data */
/* - substitution data */
struct s_tpart
{
	t_lexer_id	id;
	/* data for the token, could be: */
	/* - if lx_normal: the literal string with quote characters removed */
	/* - if lx_command: the syntax node for the command substitution */
	/* - if lx_parameter: a token for parameter expansion */
	/* - if lx_arithmetic: a token for arithmetic expansion */
	/* - if lx_backtick: the literal string for backtick command subst */
	void		*data;
	size_t		len;
	/* whether this part of the token was quoted */
	int			quote;
};

struct s_token
{
	t_token_id	id;
	t_tpart		*parts;
	size_t		count;
	char		*str;
	size_t		len;
};

struct s_source
{
	/*** internal, buffer*/
	char	*str;
	/*** internal, the current read head position*/
	size_t	off;
	/*** internal, how long the buffer is*/
	size_t	len;
	/* the current character the input currently is at*/
	int		cur;
	/* the next character after cur */
	int		nex;
	/* if the previous line read ended with \ TODO check if still needed*/
	int		esc;
	/** internal, next lines when current line's newline is escaped*/
	t_list	*lst;
	t_input	*in;
};

struct s_lexer
{
	/* the previous lexer in the stack, this is used to fill tokens with */
	/* the correct escape characters for io_here parsing */
	t_lexer		*prev;
	/* the lexer mode, could be: */
	/* - lx_normal: for heredocs and normal commands */
	/* - lx_command: for command substitutions */
	/* - lx_parameter: for parameter expansions */
	/* - lx_arithmetic: for arithmetic expansions */
	/* this will be used to figure out the meaning of quoting characters */
	/* and to determine when the end of input has been reached */
	t_lexer_id	id;
	t_source	*src;
	/* the current token being read to */
	t_token 	*tok;
	/* whether there was a \ before the current character */
	int			bslash;
	/* whether we're in a ` string */
	int			btick;
	/* the quote state, could be: */
	/* - 0: no quotes */
	/* - 1: single (') quotes */
	/* - 2: double (") quotes */
	int			quote;
	/* nesting depth of brackets, used to correctly determine the closing */
	/* paren in arithmetic expansions and command substitutions */
	int			depth;
	/* end of heredoc */
	const char	*end;
	int			here_flags;
};

/* read in the next character and also read in next if needed */
void		src_advance(t_source *src);
/* check if the end of a heredoc has been reached */
int			src_check_end(t_lexer *lex, const char *end, int flags);
ssize_t		_src_add_next(t_source *src, char **out);
ssize_t		_src_next_line(t_source *src, char **out);

/*** all of these functions should construct a sublexer and call it */
/* NORMAL: construct a normal lexer and call it with a new parser */
void		lex_normal(t_source *src, struct s_snode *node);
/* HEREDOC: construct a lexer and call lex_main */
void		lex_here(t_lexer *lex, t_token *tok, const char *end, int flags);
/* COMMAND: construct a parser and lexer and call the parser */
void		lex_command(t_lexer *lex, t_tpart *part);
/* PARAMETERS: use parameter parser for the 1st, make a lexer for the 2nd */
void		lex_parameter(t_lexer *lex, t_tpart *part);
/* ARITHMETIC: store a string but make sure to handle escapes correctly */
void		lex_arithmetic(t_lexer *lex, t_tpart *part);

/* updates the states of the lexer and or will call sublexers to process the current special character*/
int			lex_special(t_lexer *lex);

/* check if the current character is quoted: */
/* - if it is precded by a backslash */
/* - if it is inside of a single quoted string literal (except ') */
/* - if it is a special character inside a double quoted string literal: */
/* - - white space */
/* - - single quotes (') */
/* - if it is not a \ or ` in a ` string */
int			lex_quoted(t_lexer *lex);
/* check if the current character is escaped by \ given context */
/* - normally: all characters */
/* - in string or arithmetic expansion: $, `, ", \ */
/* - in heredoc or ` string: $, `, \ */
int			lex_bquoted(t_lexer *lex);
void		lex_nom(t_lexer *lex, int ch);
/* read next character using src_advance */
/* also update tok->str of all previous lexers in stack */
void		lex_advance(t_lexer *lex);
/* same as lex_advance but also update tok->str of this lexer */
void		lex_update(t_lexer *lex, int sep);
/* the main word lexing loop (not whitespace or operators) */
void		lex_main(t_lexer *lex);

/* lex an operator, tk_null if no operator, make sure to handle escapes */
t_token_id	lex_op(t_lexer *lex);
/* skip whitespace and comments */
void		lex_skip(t_lexer *lex);
/* lex a single token from a normal lexer */
int			lex_lex(t_lexer *lex, t_token *tok);
void		lex_debug(void);

/* setup a lexer with initial values */
void		lex_init(t_lexer *lex);
/* initializes a new source object */
void		src_init(t_source *src, t_input *in);
/* setup a token with initial values */
void		token_init(t_token *tok);

/* adds a token part to an existing token */
t_tpart		*token_add_part(t_token *tok, t_lexer_id id);

#endif
