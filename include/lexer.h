/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/24 14:10:27 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/03/24 14:10:28 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stddef.h>
# include "input.h"
# include "libft.h"

# ifndef SH_PR_UNEXTOKEN
#  define SH_PR_UNEXTOKEN 1
# endif

# ifndef SH_PR_UNEXEOF
#  define SH_PR_UNEXEOF 2
# endif

enum e_token_id
{
	tk_invalid = -1,
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

struct					s_snode;
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
	size_t		len;
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
	char	*str;
	size_t	off;
	size_t	len;
	int		bslash;
	int		cur;
	int		nex;
	t_list	*lst;
	t_input	*in;
};

struct s_lexer
{
	t_lexer		*prev;
	t_lexer_id	id;
	t_source	*src;
	t_token		*tok;
	int			btick;
	int			quote;
	int			depth;
	const char	*end;
	int			here_flags;
	int			error;
	int			new_part;
	int			empty_quote;
};

void		lex_append(t_lexer *lex, char **string, size_t *length, int esc);
void		src_advance(t_source *src);
int			src_check_end(t_lexer *lex, const char *end, int flags);
ssize_t		_src_add_next(t_source *src, char **out);
ssize_t		_src_next_line(t_source *src, char **out);
int			_src_cmp(const t_source *src, const char *str, int flags);
void		_src_super_nom(t_lexer *lex, const char *str);
void		_src_nom(void *ele, void *context);
void		_src_reset(t_source *src);
ssize_t		_src_add_next(t_source *src, char **out);

void		lex_here(t_lexer *lex, t_token *tok, const char *end, int flags);
void		lex_command(t_lexer *lex, t_tpart *part);
void		lex_parameter(t_lexer *lex, t_tpart *part);
void		lex_arithmetic(t_lexer *lex, t_tpart *part);

int			lex_special(t_lexer *lex);

int			lex_unquote(char *str);
int			lex_quoted(t_lexer *lex);
int			lex_quoted_int(int quote, int cur);
int			lex_bquoted(t_lexer *lex);
int			lex_bquoted_int(int quote, int cur);
void		lex_nom(t_lexer *lex, int ch);
void		lex_nom_and_skip(t_lexer *lex);
void		lex_advance(t_lexer *lex);
void		lex_update(t_lexer *lex, int sep);
int			lex_main(t_lexer *lex);

t_token_id	lex_op(t_lexer *lex);
void		lex_skip(t_lexer *lex);
int			lex_lex(t_lexer *lex, t_token *tok);
void		lex_debug(void);

void		lex_init(t_lexer *lex);
void		src_init(t_source *src, t_input *in);
void		token_init(t_token *tok);

void		token_destroy(t_token *tok);
void		token_move(t_token *dst, t_token *src);

t_tpart		*token_add_part(t_lexer *lex, t_lexer_id id, int quote);

#endif
