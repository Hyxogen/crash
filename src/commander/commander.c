/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   commander.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/24 14:47:35 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/24 17:36:30 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"

#include "memory.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>

static t_commandeer_proc
	*get_command_table(void)
{
	static t_commandeer_proc procs[46];

	procs[sx_list] = commandeer_inner;
	procs[sx_term] = commandeer_inner;
	procs[sx_pipe_sequence] = commandeer_pipe_sequence;
	procs[sx_simple_cmd] = commandeer_simple_command;
	return (procs);
}

char **_word_list_to_array(t_snode *word_list)
{
	char	**ret;
	size_t	index;

	sh_assert(word_list->type == sx_wordlist);
	index = word_list->childs_size;
	ret = sh_safe_malloc(sizeof(*ret) * (index + 1));
	ret[index] = 0;
	index -= 1;
	while (index + 1)
	{
		ret[index] = word_list->childs[index]->token.str;
		index--;
	}
	return (ret);
}

int
	commandeer_pipe_sequence(t_minishell *sh, t_snode *seq, void *data)
{

	/* TODO properly close input fds so that programs like "echo Hallo | cat" don't hang*/
	int		prev_out; 
	int		io[3];
	size_t	index;
	size_t	size;
	pid_t	pid;
	
	size = seq->childs_size;
	if (size == 1)
		return (get_command_table()[seq->childs[0]->type](sh, seq->childs[0], data));
	index = 0;
	prev_out = dup(STDIN_FILENO);
	while (index < size)
	{
		if (index + 1 < size)
			pipe(io); /* TODO check for errors */
		pid = fork();
		sh_assert(pid >= 0);
		if (pid == 0) {
			dup2(prev_out, STDIN_FILENO);
			if (index + 1 < size)
				dup2(io[1], STDOUT_FILENO);
			/* TODO not make this crash when the command doesn't exist */
			/* TODO handle complex commands */
			commandeer_simple_command_here(sh, seq->childs[index], data);
		} else {
			close(prev_out);
			prev_out = io[0];
			close(io[1]);
		}
		index++;
	}
	while (waitpid(0, NULL, WUNTRACED))
		continue ;
	sh_assert(errno == ECHILD);
	return (0);
}

int
	commandeer_simple_command_here(t_minishell *sh, t_snode *cmd_node, void *data)
{
	char	**args;

	(void) data;
	args = _word_list_to_array(cmd_node->childs[0]);
	sh_execvp(sh, args);
	perror("crash");
	kill(0, SIGABRT);
	return (-1);
}

int
	commandeer_simple_command(t_minishell *sh, t_snode *cmd_node, void *data)
{
	char	**args;
	pid_t	pid;
	int		rc;

	(void) data;
	pid = fork();
	sh_assert(pid >= 0);
	args = _word_list_to_array(cmd_node->childs[0]);
	if (pid == 0)
	{
		rc = sh_execvp(sh, args);
		perror("crash");
		kill(0, SIGABRT);
	}
	else
	{
		free(args);
		waitpid(pid, NULL, WUNTRACED);
		return (0);
	}
	return (-1);
}

int
	commandeer_inner(t_minishell *sh, t_snode *node, void *data)
{
	size_t	index;
	size_t	size;
	int		ret;

	ret = 0;
	size = node->childs_size;
	index = 0;
	while (index < size)
	{
		ret = (ret || get_command_table()[node->childs[index]->type](sh, node->childs[index], data));
		index++;
	}
	return (!!ret);
}

int
	commandeer(t_minishell *sh, t_snode *node, void *data)
{
	return (get_command_table()[node->type](sh, node, data));
}
