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
# include "minishell.h"
# include <sys/wait.h>

# ifndef SH_RETCODE_SIGNALLED_OFFSET
#  define SH_RETCODE_SIGNALLED_OFFSET 128
# endif

# define SH_BINARY_OP_COUNT 30
# define SH_UNARY_OP_COUNT 4
# define SH_OP_TOK_COUNT 35

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

typedef int						(*t_commandeer_proc)(const t_snode*,
									const int[3]);
typedef pid_t					(*t_cm_cmd_proc)(const t_snode*, const int[3]);
typedef pid_t					(*t_command)(const t_snode *, const int[3]);
typedef int						(*t_cm_cmd_wait)(pid_t pid);
typedef long					(*t_cm_eval_proc)(const char *ptr,
									long a, long b, long c);
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
typedef struct s_arith_operator	t_arith_op;
typedef struct s_arith_optok	t_arith_optok;
typedef struct s_arith_value	t_arith_value;
typedef struct s_exp_param_ctx	t_exp_param_ctx;
typedef struct s_wildcard_ctx	t_wildcard_ctx;

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
	int		tilde_expanded;
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
	int				wildcard;
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
	int				is_fake;
	int				recursion_level;
};

struct s_arith_operator {
	t_arith_token_id	token;
	int					prec;
	t_associativity		ass;
	t_cm_eval_proc		proc;
	int					is_assignment;
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

struct s_exp_param_ctx {
	int				*info;
	char			*pattern;
	int				long_mode;
	size_t			i;
	size_t			j;
	size_t			best;
	char			*str;
	int				ch;
	int				match;
	t_pattern_node	*comp_pattern;
};

struct s_wildcard_ctx {
	int		slash;
	size_t	i;
	int		has_match;
	char	***out;
};

int				command(t_snode *cmd_node, int io[3]);

pid_t			cm_convert_retcode(int rc);
pid_t			cm_simple_cmd_command(const t_snode *cmd_node, const int io[3]);
pid_t			commandeer_simple_command(const t_snode *command,
					const int io[SH_STDIO_SIZE]);
pid_t			cm_if_clause(const t_snode *ifnode,
					const int io[3]);
pid_t			cm_for_clause(const t_snode *ifnode,
					const int io[3]);
pid_t			cm_case_clause(const t_snode *ifnode,
					const int io[3]);
pid_t			cm_while_until_clause(const t_snode *ifnode,
					const int io[3]);
pid_t			cm_function(const t_snode *ifnode,
					const int io[3]);
pid_t			cm_function_define(const t_snode *ifnode,
					const int io[3]);
pid_t			cm_compound_list(const t_snode *node,
					const int io[SH_STDIO_SIZE]);

pid_t			execute_command_nofork(const t_snode *command,
					const int io[SH_STDIO_SIZE]);
pid_t			execute_command_fork(const t_snode *command,
					const int io[SH_STDIO_SIZE]);
int				create_pipe_and_execute_command(const t_snode *command,
					pid_t *command_pid, int input_fd);
int				execute_commands_recursive(const t_snode *pipe_seq,
					const int final_out_fd, int previous_out_fd, size_t index);
int				execute_pipe_seq(const t_snode *list_node,
					const int io[3]);
int				cm_and_if(const t_snode *node,
					const int io[3]);
int				cm_or_if(const t_snode *node,
					const int io[3]);
int				commandeer_inner(const t_snode *node,
					const int io[3]);
int				commandeer(const t_snode *node,
					const int io[3]);
int				commandeer_term(const t_snode *node,
					const int io[SH_STDIO_SIZE]);

int				sh_execvp(char **argv);

int				command_restore_internal_redirects(const int io[SH_STDIO_SIZE],
					const int old_io[SH_STDIO_SIZE]);
int				command_setup_internal_redirects(const t_snode *redirect_list,
					const int io[SH_STDIO_SIZE], int old_io[SH_STDIO_SIZE]);

int				_cm_get_redi_flags(t_syntax_id type);
int				_cm_redi_get_from_index(const t_snode *redi_node);
int				_cm_redi_get_from(const t_snode *redi_node);
int				_cm_setup_builtin_redirects(const t_snode *redi_list,
					int io[3]);
int				_cm_handle_builtin_redi_node_noerr(const t_snode *node,
					char *filen, int io[3]);
int				_cm_handle_process_node_noerr(const t_snode *node, char *filen);
/* This functions only exists for norminette */
/* It just calls _cm_get_redi_flags */
int				fl(t_syntax_id type);
int				_cm_create_and_write_here(const char *str,
					int skip_leading_tabs);
int				_cm_setup_process_redirects(const t_snode *redi_list);
int				_cm_setup_builtin_redirects(const t_snode *redi_list,
					int io[3]);
int				cm_expand_list(t_expand *exp, const t_token *token, int mode);
char			**cm_expand(const t_token *token, int ***quote, int mode);
char			*cm_expand_str(const t_token *token,
					int **quote, int ch, int mode);
int				expand_param(t_expand *exp, t_token *token, int mode);
int				expand_command(t_expand *exp, t_snode *node);
int				expand_backtick(t_expand *exp, char *str);
int				expand_arith(t_expand *exp, t_token *token);
int				expand_special_asterisk(t_expand *exp, char *key);
int				expand_special_at(t_expand *exp, char *key);
int				expand_special_sharp(t_expand *exp, char *key, long i);
int				expand_special_qmark(t_expand *exp, char *key);
int				expand_special_minus(t_expand *exp, char *key);
int				expand_special_dollar(t_expand *exp, char *key);
int				expand_special_bang(t_expand *exp, char *key);
int				expand_special_digit(t_expand *exp, char *key, long i);
size_t			expand_key_length(const char *param);
int				expand_empty(t_expand *exp, int empty_is_null);
size_t			expand_length(t_expand *exp);
int				expand_error(t_param_ctx *ctx, size_t i);
int				expand_special(t_expand *exp, char *key);
int				expand_assign(t_expand *exp,
					t_param_ctx *ctx, size_t i, int mode);
int				expand_right(t_expand *exp,
					t_param_ctx *ctx, size_t i, int mode);
int				expand_promote(t_expand *exp,
					t_expand *tmp);
void			expand_pattern_percent(t_exp_param_ctx *ctxx);
void			expand_pattern_hash(t_exp_param_ctx *ctxx, t_expand *exp);

pid_t			cm_unimplemented_cmd_command(const t_snode *node,
					const int io[3]);

char			**cm_word_list_to_array(const t_snode *word_list);

void			try_reap_all_childs(void);

int				status_code_to_return_code(int status_code);
int				internal_pid_to_return_code(pid_t command_pid);
pid_t			return_code_to_internal_pid(int return_code);
int				process_wait_and_get_return_code(pid_t pid);
int				wait_and_get_return_code(pid_t command_pid);

int				sh_cm_run(t_input *in);
int				match_pattern(const char *str, const char *pattern,
					const char *info);

void			expansion_destroy(t_expand *exp);
void			expansion_init(t_expand *exp);
void			expansion_add_part(t_expand *exp, char **str, int quote);
void			expansion_copy_parts(t_expand *dst, t_expand *src);


void			expand_tilde(t_epart *part, int first, int last, int ass);
int				cm_expand_list(t_expand *exp, const t_token *token, int mode);
char			*cm_expand_str_end(int **quote, t_stringlst *lst);
char			*cm_expand_str(const t_token *token, int **quote, int ch, int mode);
char			**cm_expand(const t_token *token, int ***quotes, int mode);
char			*expand_tilde1(char *str, t_stringlst *lst, int last, int ass);
void			expand_str_add_part(t_stringlst *lst, t_epart *part, int delim);
void			expand_collate(t_expand *e, t_stringlst *l);
void			expand_split(t_stringlst *lst, int *new, char *str, int quot);
void			expand_add_str(t_stringlst *lst, int *new, char *str, int quot);
void			expand_add(t_stringlst *lst, int *new, int tmp, int quot);
int				expand_part(t_expand *exp, const t_tpart *part, int mode);

int				_pattern_process_collating_class(char **pattern,
					t_pattern_node *node, char *ch);
int				_pattern_process_equivalence_class(char **pattern,
					t_pattern_node *node);
int				_pattern_process_char_class(char **pattern,
					t_pattern_node *node);
int				_pattern_process_brackets(char **pattern,
					t_pattern_node *node, int moved, int local_moved);
t_pattern_node	*pattern_compile(char *pattern, int *info);
int				pattern_match(const char *str,
					t_pattern_node *pattern, int filename);
void			pattern_destroy(t_pattern_node *node);
void			pattern_debug_print_node(t_pattern_node *node);
void			pattern_debug_print_chain(t_pattern_node *head);

t_envvar		*_do_assignment(const char *ass, int is_tmp);
int				_do_assignments(const t_snode *ass_list, int is_tmp);

void			handle_execvp_error(const char *name, int error);
char			**command_get_arguments(const t_snode *command);
void			handle_execvp_error(const char *name, int error);
int				command_should_execute(const t_snode *command);
int				get_argument_count(char *const *argv);
int				close_nostd_fd(int fd);
void			close_or_dup2_fd(int fromfd, int tofd);
int				command_setup_internal_redirects(const t_snode *redirect_list,
					const int io[SH_STDIO_SIZE], int old_io[SH_STDIO_SIZE]);
int				command_setup_external_redirects(const t_snode *redirect_list,
					const int io[SH_STDIO_SIZE]);
int				command_restore_internal_redirects(const int io[SH_STDIO_SIZE],
					const int old_io[SH_STDIO_SIZE]);
pid_t			execute_builtin(const t_snode *command,
					const t_builtin *function, char **argv,
					const int io[SH_STDIO_SIZE]);
pid_t			execute_function(const t_snode *command,
					t_snode *body, char **argv,
					const int io[SH_STDIO_SIZE]);
pid_t			find_and_execute_builtin(const t_snode *command,
					char **argv, const int io[SH_STDIO_SIZE]);
pid_t			find_and_execute_function(const t_snode *command,
					char **argv, const int io[SH_STDIO_SIZE]);
pid_t			find_and_execute_utility(const t_snode *command,
					char **argv, const int io[SH_STDIO_SIZE]);

t_arith_op		*get_binary_operators(void);
t_arith_optok	*get_operator_tokens(void);
t_arith_op		*get_unary_operators(void);
void			arith_binary_init1(void);
void			arith_binary_init2(void);
void			arith_binary_init3(void);
void			arith_binary_init4(void);
void			arith_binary_init5(void);
void			arith_binary_init6(void);
void			arith_binary_init(void);
void			arith_optok_init(void);
void			arith_init(void);
t_arith_op		*arith_get_unary_op(t_arith_token_id id);
t_arith_op		*arith_get_binary_op(t_arith_token_id id);
void			arith_binary_handle_notquest(t_arith_parser *pr,
					t_arith_value *lhs, const t_arith_op *op,
					t_arith_op **lookahead);
void			arith_binary_handle_quest(t_arith_parser *pr,
					t_arith_value *lhs, const t_arith_op *op,
					t_arith_op **lookahead);
long			arith_plus(const char *str,
					long lhs, long rhs, long c);
long			arith_plus_eq(const char *str,
					long rhs, long b, long c);
long			arith_minus(const char *str,
					long lhs, long rhs, long c);
long			arith_minus_eq(const char *str,
					long lhs, long rhs, long c);
long			arith_decrement(const char *str,
					long a, long b, long c);
long			arith_modulo(const char *str,
					long dividend, long divisor, long c);
long			arith_modulo_eq(const char *str,
					long lhs, long rhs, long c);
long			arith_multiple_eq(const char *str,
					long lhs, long rhs, long c);
long			arith_multiply(const char *str,
					long lhs, long rhs, long c);
long			arith_multiple_eq(const char *str,
					long lhs, long rhs, long c);
long			arith_divide(const char *str,
					long dividend, long divisor, long c);
long			arith_divide_eq(const char *str,
					long lhs, long rhs, long c);
long			arith_ternary(const char *str,
					long lhs, long rhs, long super_rhs);
long			arith_shift_left(const char *ptr,
					long lhs, long rhs, long c);
long			arith_shift_left_eq(const char *str,
					long lhs, long rhs, long c);
long			arith_shift_right(const char *ptr,
					long lhs, long rhs, long c);
long			arith_shift_right_eq(const char *str,
					long lhs, long rhs, long c);
long			arith_less_than(const char *str,
					long lhs, long rhs, long c);
long			arith_less_than_or_eq(const char *str,
					long lhs, long rhs, long c);
long			arith_greater_than(const char *str,
					long lhs, long rhs, long c);
long			arith_greater_than_or_eq(const char *str,
					long lhs, long rhs, long c);
long			arith_equal(const char *str,
					long lhs, long rhs, long c);
long			arith_not_equal(const char *str,
					long lhs, long rhs, long c);
long			arith_bitwise_or(const char *ptr,
					long lhs, long rhs, long c);
long			arith_bitwise_and(const char *ptr,
					long lhs, long rhs, long c);
long			arith_bitwise_xor(const char *ptr,
					long lhs, long rhs, long c);
long			arith_bitwise_or_eq(const char *str,
					long lhs, long rhs, long c);
long			arith_bitwise_and_eq(const char *str,
					long lhs, long rhs, long c);
long			arith_bitwise_xor_eq(const char *str,
					long lhs, long rhs, long c);
long			arith_logical_and(const char *str,
					long lhs, long rhs, long c);
long			arith_logical_or(const char *str,
					long lhs, long rhs, long c);
long			arith_identity(const char *str,
					long lhs, long b, long c);
long			arith_negate(const char *str,
					long lhs, long b, long c);
long			arith_logical_not(const char *str,
					long lhs, long b, long c);
long			arith_bitwise_not(const char *str,
					long lhs, long b, long c);
long			arith_assign(const char *str,
					long old_value, long value, long c);
t_arith_value	arith_parse_binary(t_arith_parser *pr,
					t_arith_value lhs, int min_prec);
t_arith_value	arith_parse_unary(t_arith_parser *pr);
int				arith_lex(t_arith_lexer *lex, t_arith_token *tok);
long			expand_arith_str(const char *str,
					int *error, int recursion_level);

void			cm_wildcard_add(char *filename, char ***out);
char			*cm_wildcard_join(char *prefix, char *name, char *suffix);
void			cm_wildcard(const char *path, int *info, char ***out);
char			**cm_wildcard_expand(const t_token *token);
#endif
