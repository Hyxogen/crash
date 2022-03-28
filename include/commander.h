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

typedef int	(*t_commandeer_proc)(t_minishell*, t_snode*, void*);
typedef pid_t (*t_cm_cmd_proc)(t_minishell*, t_snode*, int, int);
typedef int (*t_cm_cmd_wait)(pid_t pid);
typedef struct s_pipe_seq_ctx	t_pipe_seq_ctx;
typedef struct s_cmd_base		t_cmd_base;
typedef struct s_simple_cmd_ctx	t_simple_cmd_ctx;

struct s_pipe_seq_ctx {
	int	run_in_background;
	int	prev_out_fd;
};

struct s_simple_cmd_ctx {
	t_minishell	*sh;
	t_snode		*cmd_node;
	char		**args;
	int			fd_in;
	int			fd_out;
};

struct s_cmd_base {
	t_cm_cmd_proc	on_command;
	t_cm_cmd_wait	wait;
};

int			commander_setup(t_minishell *sh);

int			command(t_minishell *sh, t_snode *cmd_node, int io[3]);

pid_t		cm_simple_cmd_command(t_minishell *sh, t_snode *cmd_node, int fd_in, int fd_out);
int			cm_simple_cmd_wait(pid_t pid);

int			commandeer_pipe_sequence(t_minishell *sh, t_snode *list_node, void *data);
int			commandeer_inner(t_minishell *sh, t_snode *node, void *data);
int			commandeer(t_minishell *sh, t_snode *node, void *data);

int			sh_execvp(t_minishell *sh, char **argv);

int			_cm_setup_redirects(t_minishell *sh, t_snode *redi_list);
int			_cm_setup_redirects(t_minishell *sh, t_snode *redi_list);
char		**cm_expand(t_minishell *sh, t_token *token);
char		*cm_expand_param(t_minishell *sh, t_token *token);

pid_t		cm_unimplemented_cmd_command(t_minishell *sh, t_snode *node, int fd_in, int fd_out);
int			cm_unimplemented_cmd_wait(pid_t pid);

int			cm_enable_reaper(t_minishell *sh);
int			cm_disable_reaper(t_minishell *sh);
#endif
