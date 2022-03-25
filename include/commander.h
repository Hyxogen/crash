/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   commander.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/24 14:45:08 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/25 14:06:14 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/* We have fun around here */
#ifndef COMMANDER_H
# define COMMANDER_H

# include "parser.h"
# include "lexer.h"
# include <sys/wait.h>

typedef int	(*t_commandeer_proc)(t_minishell*, t_snode*, void*);

typedef struct s_child {
	int		io[3];
	pid_t	pid;
}	t_child;

char		**_word_list_to_array(t_snode *word_list);
		
int			command(t_minishell *sh, t_snode *cmd_node, int io[3]);
		
int			commandeer_pipe_sequence(t_minishell *sh, t_snode *list_node, void *data);
int			commandeer_inner(t_minishell *sh, t_snode *node, void *data);
int			commandeer(t_minishell *sh, t_snode *node, void *data);

int			sh_execvp(t_minishell *sh, char **argv);

int			_cm_setup_redirects(t_minishell *sh, t_snode *redi_list);
int			_cm_setup_redirects(t_minishell *sh, t_snode *redi_list);
const char	*cm_expand(t_token *token);

#endif
