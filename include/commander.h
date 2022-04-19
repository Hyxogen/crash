/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   commander.c                                      -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:22:30 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:22:30 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

/* We have fun around here */
#ifndef COMMANDER_H
# define COMMANDER_H

# include "parser.h"
# include "lexer.h"
# include <sys/wait.h>

# ifndef SH_STDIN_INDEX
#  define SH_STDIN_INDEX 0
# endif
# ifndef SH_STDOUT_INDEX
#  define SH_STDOUT_INDEX 1
# endif
# ifndef SH_STDERR_INDEX
#  define SH_STDERR_INDEX 2
# endif

enum e_associativity {
	ass_left,
	ass_right
};

enum e_arith_token_id {
	ar_tk_eof = -1,
	ar_tk_null,
	ar_tk_ident,
	ar_tk_const,
	ar_op_lparen,
	ar_op_rparen,
	ar_op_plus,
	ar_op_minus,
	ar_op_tilde,
	ar_op_excl,
	ar_op_star,
	ar_op_slash,
	ar_op_perc,
	ar_op_less_less,
	ar_op_great_great,
	ar_op_less,
	ar_op_great,
	ar_op_less_eq,
	ar_op_great_eq,
	ar_op_eq_eq,
	ar_op_excl_eq,
	ar_op_amp,
	ar_op_caret,
	ar_op_pipe,
	ar_op_amp_amp,
	ar_op_pipe_pipe,
	ar_op_quest,
	ar_op_colon,
	ar_op_eq,
	ar_op_star_eq,
	ar_op_slash_eq,
	ar_op_perc_eq,
	ar_op_plus_eq,
	ar_op_minus_eq,
	ar_op_less_less_eq,
	ar_op_great_great_eq,
	ar_op_amp_eq,
	ar_op_caret_eq,
	ar_op_pipe_eq
};

typedef int						(*t_commandeer_proc)(const t_snode*, const int[3]);
typedef pid_t					(*t_cm_cmd_proc)(const t_snode*, const int[3]);
typedef int						(*t_cm_cmd_wait)(pid_t pid);
typedef long					(*t_cm_eval_proc)(const char *ptr, long a, long b, long c);
typedef enum e_associativity	t_associativity;
typedef enum e_arith_token_id	t_arith_token_id;
typedef struct s_cmd_base		t_cmd_base;
typedef struct s_simple_cmd_ctx	t_simple_cmd_ctx;
typedef struct s_pipe_ctx		t_pipe_ctx;
typedef struct s_pipe_ctx		t_pipe_ctx;
typedef struct s_param_ctx		t_param_ctx;
typedef struct s_epart			t_epart;
typedef struct s_expand			t_expand;
typedef struct s_pattern_node	t_pattern_node;
typedef struct s_arith_token	t_arith_token;
typedef struct s_arith_lexer	t_arith_lexer;
typedef struct s_arith_parser	t_arith_parser;
typedef struct s_arith_operator	t_arith_operator;
typedef struct s_arith_optok	t_arith_optok;
typedef struct s_arith_value	t_arith_value;

struct s_simple_cmd_ctx {
	const t_snode		*cmd_node;
	char				**args;
	int					io[3];
};

struct s_pipe_ctx {
	const t_snode	*node;
	int				begin_in;
	int				end_out;
};

struct s_epart {
	char	**str;
	int		quote;
	int		normal;
	int		array;
};

struct s_expand {
	t_epart	*parts;
	size_t	count;
};

struct s_param_ctx {
	t_token	*token;
	char	*key;
	size_t	i;
};

struct s_pattern_node {
	char			chars[256];
	int				invert;
	int				infinite;
	t_pattern_node	*next;
};

struct s_arith_token {
	t_arith_token_id	id;
	const char			*str;
	size_t				size;
};

struct s_arith_lexer {
	t_arith_token	*tok;
	const char		*str;
};

struct s_arith_parser {
	t_arith_lexer	*lex;
	t_arith_token	next;
	int				error;
};

struct s_arith_operator {
	t_arith_token_id	token;
	int					prec;
	t_associativity		ass;
	t_cm_eval_proc		proc;
};

struct s_arith_optok {
	const char			*str;
	t_arith_token_id	id;
};

struct s_arith_value {
	long		value;
	const char	*str;
	size_t		size;
};

int				command(t_snode *cmd_node, int io[3]);

void			_cm_close_nostd(int fd);
/* Closes all the non std filedescriptors that differ between the orginal and redirect */
void			cm_close_nstd_nred(const int original[3], const int redirect[3]);

pid_t			cm_convert_retcode(int rc);
pid_t			cm_simple_cmd_command(const t_snode *cmd_node, const int io[3]);
pid_t			cm_if_clause(const t_snode *ifnode, const int io[3]);
pid_t			cm_for_clause(const t_snode *ifnode, const int io[3]);
pid_t			cm_case_clause(const t_snode *ifnode, const int io[3]);
pid_t			cm_while_until_clause(const t_snode *ifnode, const int io[3]);
pid_t			cm_function(const t_snode *ifnode, const int io[3]);
pid_t			cm_function_define(const t_snode *ifnode, const int io[3]);

int				commandeer_pipe_sequence(const t_snode *list_node, const int io[3]);
int				cm_and_if(const t_snode *node, const int io[3]);
int				cm_or_if(const t_snode *node, const int io[3]);
int				commandeer_inner(const t_snode *node, const int io[3]);
int				commandeer(const t_snode *node, const int io[3]);

int				sh_execvp(char **argv);

int				_cm_get_redi_flags(t_syntax_id type);
int				_cm_create_and_write_here(const char *str, int skip_leading_tabs);
int				_cm_setup_process_redirects(const t_snode *redi_list);
int				_cm_setup_builtin_redirects(const t_snode *redi_list, int io[3]);
int				cm_expand_list(t_expand *exp, const t_token *token);
char			**cm_expand(const t_token *token);
char			*cm_expand_str(const t_token *token, int **quote, int ch);
int				expand_param(t_expand *exp, t_token *token);
int				expand_command(t_expand *exp, t_snode *node);
int				expand_backtick(t_expand *exp, char *str);
int				expand_arith(t_expand *exp, t_token *token);

pid_t			cm_unimplemented_cmd_command(const t_snode *node, const int io[3]);

char			**cm_word_list_to_array(const t_snode *word_list);

int				cm_enable_reaper(void);
int				cm_disable_reaper(void);

int				sh_cm_run(t_input *in);
int				match_pattern(const char *str, const char *pattern, const char *info);

void			expansion_destroy(t_expand *exp);
void			expansion_init(t_expand *exp);
void			expansion_add_part(t_expand *exp, char **str, int quote);
void			expansion_copy_parts(t_expand *dst, t_expand *src);

int				_pattern_process_collating_class(char **pattern, t_pattern_node *node, char *ch);
int				_pattern_process_equivalence_class(char **pattern, t_pattern_node *node);
int				_pattern_process_char_class(char **pattern, t_pattern_node *node);
int				_pattern_process_brackets(char **pattern, t_pattern_node *node, int moved, int local_moved);
t_pattern_node	*_pattern_generate(char *pattern, int *info);
int				pattern_match(const char *str, char *pattern, int *info);
void			_pattern_destroy(t_pattern_node *node);
void			pattern_debug_print_node(t_pattern_node *node);
void			pattern_debug_print_chain(t_pattern_node *head);

long			arith_plus(const char *str, long lhs, long rhs, long c);
long			arith_get_val(const char *key);
#endif
