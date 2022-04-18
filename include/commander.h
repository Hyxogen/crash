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

enum e_arith_id {
	ar_primary,
	ar_conditional,
	ar_assign,
	ar_op_mul,
	ar_op_div,
	ar_op_mod,
	ar_op_sub,
	ar_op_add,
	ar_bit_not,
	ar_bit_shl,
	ar_bit_shr,
	ar_bit_and,
	ar_bit_xor,
	ar_bit_or,
	ar_rel_lt,
	ar_rel_gt,
	ar_rel_eq
};

typedef int						(*t_commandeer_proc)(t_snode*, const int[3]);
typedef pid_t					(*t_cm_cmd_proc)(t_snode*, const int[3]);
typedef int						(*t_cm_cmd_wait)(pid_t pid);
typedef enum e_arith_id			t_arith_id;
typedef struct s_cmd_base		t_cmd_base;
typedef struct s_simple_cmd_ctx	t_simple_cmd_ctx;
typedef struct s_pipe_ctx		t_pipe_ctx;
typedef struct s_pipe_ctx		t_pipe_ctx;
typedef struct s_param_ctx		t_param_ctx;
typedef struct s_epart			t_epart;
typedef struct s_expand			t_expand;
typedef struct s_pattern_node	t_pattern_node;
typedef struct s_arith_node		t_arith_node;

struct s_simple_cmd_ctx {
	t_snode		*cmd_node;
	char		**args;
	int			io[3];
};

struct s_pipe_ctx {
	t_snode	*node;
	int		begin_in;
	int		end_out;
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

struct s_arith_node {
	t_arith_id		id;
	t_arith_node	*children;
	size_t			children_size;
};

int				command(t_snode *cmd_node, int io[3]);

void			_cm_close_nostd(int fd);
/* Closes all the non std filedescriptors that differ between the orginal and redirect */
void			cm_close_nstd_nred(const int original[3], const int redirect[3]);

pid_t			cm_convert_retcode(int rc);
pid_t			cm_simple_cmd_command(t_snode *cmd_node, const int io[3]);
pid_t			cm_if_clause(t_snode *ifnode, const int io[3]);
pid_t			cm_for_clause(t_snode *ifnode, const int io[3]);
pid_t			cm_case_clause(t_snode *ifnode, const int io[3]);
pid_t			cm_while_until_clause(t_snode *ifnode, const int io[3]);
pid_t			cm_function(t_snode *ifnode, const int io[3]);
pid_t			cm_function_define(t_snode *ifnode, const int io[3]);

int				commandeer_pipe_sequence(t_snode *list_node, const int io[3]);
int				commandeer_inner(t_snode *node, const int io[3]);
int				commandeer(t_snode *node, const int io[3]);

int				sh_execvp(char **argv);

int				_cm_setup_process_redirects(const t_snode *redi_list);
int				_cm_setup_builtin_redirects(const t_snode *redi_list, int io[3]);
int				cm_expand_list(t_expand *exp, t_token *token);
char			**cm_expand(t_token *token);
char			*cm_expand_str(t_token *token, int **quote, int ch);
int				expand_param(t_expand *exp, t_token *token);
int				expand_command(t_expand *exp, t_snode *node);
int				expand_backtick(t_expand *exp, char *str);
int				expand_arith(t_expand *exp, t_token *token);

pid_t			cm_unimplemented_cmd_command(t_snode *node, const int io[3]);

char			**cm_word_list_to_array(t_snode *word_list);

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
#endif
