#ifndef LEXER_H
# define LEXER_H

#include <stddef.h>
#include "parser.h"

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
	lx_arithmetic
};

typedef enum e_token_id	t_token_id;
typedef enum e_lexer_id	t_lexer_id;
typedef struct s_tpart	t_tpart;
typedef struct s_token	t_token;
typedef struct s_lexer	t_lexer;
typedef struct s_source	t_source;

struct s_tpart
{
	t_lexer_id	id;
	void		*data;
	int			quote;
};

struct s_token
{
	t_token_id	id;
	t_tpart		*parts;
	size_t		count;
};

struct s_source
{

};

struct s_lexer
{
	t_lexer_id	id;
	t_source	*src;
	t_token 	*tok;
	int			quote;
	int			depth;
	const char	*end;
};

t_token	*lex_token(t_source *src, const char *end);
t_snode	*lex_snode(t_source *src);
t_tpart *lex_tpart(t_source *src, t_lexer_id id);

#endif