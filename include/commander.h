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

typedef int						(*t_commandeer_proc)(t_minishell*, t_snode*, const int[3]);
typedef pid_t					(*t_cm_cmd_proc)(t_minishell*, t_snode*, const int[3], int);
typedef int						(*t_cm_cmd_wait)(pid_t pid);
typedef struct s_pipe_seq_ctx	t_pipe_seq_ctx;
typedef struct s_cmd_base		t_cmd_base;
typedef struct s_simple_cmd_ctx	t_simple_cmd_ctx;
typedef struct s_pipe_ctx		t_pipe_ctx;
typedef struct s_param_ctx		t_param_ctx;
typedef struct s_epart			t_epart;
typedef struct s_expand			t_expand;
typedef struct s_pattern_node	t_pattern_node;

struct s_simple_cmd_ctx {
	t_minishell	*sh;
	t_snode		*cmd_node;
	char		**args;
	int			io[3];
	int			closefd;
};

struct s_pipe_ctx {
	t_snode	*pipe_node;
	int		io[3];
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

int		commander_setup(t_minishell *sh);

int		command(t_minishell *sh, t_snode *cmd_node, int io[3]);

void	_cm_close_nostd(int fd);

pid_t	cm_convert_retcode(int rc);
pid_t	cm_simple_cmd_command(t_minishell *sh, t_snode *cmd_node, const int io[3], int closefd);
pid_t	cm_if_clause(t_minishell *sh, t_snode *ifnode, const int io[3]);
pid_t	cm_for_clause(t_minishell *sh, t_snode *ifnode, const int io[3]);
pid_t	cm_case_clause(t_minishell *sh, t_snode *ifnode, const int io[3]);

int		commandeer_pipe_sequence(t_minishell *sh, t_snode *list_node, const int io[3]);
int		commandeer_inner(t_minishell *sh, t_snode *node, const int io[3]);
int		commandeer(t_minishell *sh, t_snode *node, const int io[3]);

int		sh_execvp(t_minishell *sh, char **argv);

int		_cm_setup_process_redirects(t_minishell *sh, t_snode *redi_list);
int		_cm_setup_builtin_redirects(t_minishell *sh, t_snode *redi_list, int io[3]);
char	**cm_expand(t_minishell *sh, t_token *token);
char	*cm_expand_str(t_minishell *sh, t_token *token, int **quote, int ch);
int		expand_param(t_minishell *sh, t_expand *exp, t_token *token);
int		expand_command(t_minishell *sh, t_expand *exp, t_snode *node);
int		expand_backtick(t_minishell *sh, t_expand *exp, char *str);
int		expand_arith(t_minishell *sh, t_expand *exp, t_token *token);

pid_t	cm_unimplemented_cmd_command(t_minishell *sh, t_snode *node, const int io[3]);

char	**cm_word_list_to_array(t_minishell *sh, t_snode *word_list);

int		cm_enable_reaper(t_minishell *sh);
int		cm_disable_reaper(t_minishell *sh);

int		sh_cm_run(t_minishell *sh, t_input *in);
int		match_pattern(const char *str, const char *pattern, const char *info);

void	expansion_destroy(t_expand *exp);
void	expansion_init(t_expand *exp);
void	expansion_add_part(t_expand *exp, char **str, int quote);

#endif
