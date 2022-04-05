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
typedef pid_t					(*t_cm_cmd_proc)(t_minishell*, t_snode*, const int[3]);
typedef int						(*t_cm_cmd_wait)(pid_t pid);
typedef struct s_pipe_seq_ctx	t_pipe_seq_ctx;
typedef struct s_cmd_base		t_cmd_base;
typedef struct s_simple_cmd_ctx	t_simple_cmd_ctx;
typedef struct s_pipe_ctx		t_pipe_ctx;
typedef struct s_token_ctx		t_token_ctx;

struct s_simple_cmd_ctx {
	t_minishell	*sh;
	t_snode		*cmd_node;
	char		**args;
	int			io[3];
};

struct s_pipe_ctx {
	t_snode	*pipe_node;
	int		io[3];
};

struct s_token_ctx {
	char	***list;
	char	**fields;
};

int		commander_setup(t_minishell *sh);

int		command(t_minishell *sh, t_snode *cmd_node, int io[3]);

pid_t	cm_convert_retcode(int rc);
pid_t	cm_simple_cmd_command(t_minishell *sh, t_snode *cmd_node, const int io[3]);
pid_t	cm_if_clause(t_minishell *sh, t_snode *ifnode, const int io[3]);
pid_t	cm_for_clause(t_minishell *sh, t_snode *ifnode, const int io[3]);
pid_t	cm_case_clause(t_minishell *sh, t_snode *ifnode, const int io[3]);

int		commandeer_pipe_sequence(t_minishell *sh, t_snode *list_node, const int io[3]);
int		commandeer_inner(t_minishell *sh, t_snode *node, const int io[3]);
int		commandeer(t_minishell *sh, t_snode *node, const int io[3]);

int		sh_execvp(t_minishell *sh, char **argv);

int		_cm_setup_process_redirects(t_minishell *sh, t_snode *redi_list);
int		_cm_setup_builtin_redirects(t_minishell *sh, t_snode *redi_list, int io[3]);
char	**cm_expand(t_minishell *sh, t_token *token, int nosplit);
char	*cm_expand_command(t_minishell *sh, t_snode *node);
char	*cm_expand_backtick(t_minishell *sh, char *str);
char	**cm_expand_param(t_minishell *sh, t_token *token);
char	**cm_expand_special(t_minishell *sh, const char *key);
char	**param_pattern(t_minishell *sh, t_token *tok, char **str, size_t i);

pid_t	cm_unimplemented_cmd_command(t_minishell *sh, t_snode *node, const int io[3]);

char	**cm_word_list_to_array(t_minishell *sh, t_snode *word_list);

int		cm_enable_reaper(t_minishell *sh);
int		cm_disable_reaper(t_minishell *sh);

int		sh_cm_run(t_minishell *sh, t_input *in);

#endif
