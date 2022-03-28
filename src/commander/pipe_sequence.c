/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipe_sequence.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/25 16:22:25 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/03/28 10:51:19 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"

#include "minishell.h"
#include "memory.h"
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>


/* for perror */
#include <stdio.h>

static int
	_get_exit_code(int status_code)
{
	if (WIFSIGNALED(status_code))
		return (128 + WTERMSIG(status_code));
	return (WEXITSTATUS(status_code));
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

void
	_sh_execvp_error_handler(const char *name, int error)
{
	if (error == ENOENT)
	{
		fprintf(stderr, "CraSH: Could not find executable: \"%s\"\n", name);
		exit(127);
	}
	else if (error == EACCES)
	{
		fprintf(stderr, "CraSH: \"%s\" Permission denied\n", name);
		exit(127);
	}
	else
	{
		fprintf(stderr,
			"CraSH: An unknown error ocurred in attempting to execute: \"%s\". Crashing.\n%s\n", name, strerror(error));
		sh_abort();
	}   
}

pid_t
	cm_simple_cmd_command(t_minishell *sh, t_snode *cmd_node, int fd_in, int fd_out)
{
	pid_t	pid;
	char	**args;

	(void) cmd_node;
	pid = sh_fork();
	if (pid == 0)
	{
		args = _word_list_to_array(cmd_node->childs[0]);
		dup2(fd_in, STDIN_FILENO);
		dup2(fd_out, STDOUT_FILENO);
		_cm_setup_redirects(sh, cmd_node->childs[1]);
		if (fd_in != STDIN_FILENO)
			close(fd_in);
		if (fd_out != STDOUT_FILENO)
			close(fd_out);
		sh_execvp(sh, args);
		_sh_execvp_error_handler(args[0], errno);
	}
	return (pid);
}

int
	cm_simple_cmd_wait(pid_t pid)
{
	int		status;

	sh_assert(waitpid(pid, &status, WUNTRACED) > 0);
	return (_get_exit_code(status));
}
/*
int
	_commandeer_pipe_sequence(t_minishell *sh, t_snode *seq_node, int fd_out)
{
	size_t	index;
	size_t	count;
	int		io[2];
	int		prev_out;

	index = 0;
	prev_out = STDIN_FILENO;
	count = seq_node->childs_size;
	while (index < count)
	{
		if (index < (count - 1))
		{
			sh_pipe(io);
			commandeer_simple_cmd(sh, seq_node->childs[index], prev_out, io[1]);
			sh_close(io[1]);
		}
		else
			commandeer_simple_cmd(sh, seq_node->childs[index], prev_out, fd_out);
		if (prev_out != STDIN_FILENO)
			sh_close(prev_out);
		prev_out = io[0];
		index++;
	}
	if (fd_out != STDOUT_FILENO)
		sh_close(fd_out);
	return (0);
}
*/

static const t_cmd_base
	*_get_commandeer_cmd_procs(void)
{
	static const t_cmd_base	cmds[] = {
		{cm_simple_cmd_command, cm_simple_cmd_wait},
		{cm_unimplemented_cmd_command, cm_unimplemented_cmd_wait},
		{cm_unimplemented_cmd_command, cm_unimplemented_cmd_wait},
		{cm_unimplemented_cmd_command, cm_unimplemented_cmd_wait},
		{cm_unimplemented_cmd_command, cm_unimplemented_cmd_wait},
		{cm_unimplemented_cmd_command, cm_unimplemented_cmd_wait},
		{cm_unimplemented_cmd_command, cm_unimplemented_cmd_wait}
	};

	return (cmds);
}

pid_t
	cm_unimplemented_cmd_command(t_minishell *sh, t_snode *node, int fd_in, int fd_out)
{
	(void) sh;
	(void) node;
	(void) fd_in;
	(void) fd_out;

	fprintf(stderr, "Executing this command type is not implemented yet\n");
	return (-1);
}

int
	cm_unimplemented_cmd_wait(pid_t pid)
{
	(void) pid;
	fprintf(stderr, "Waiting for this command type is not implemented yet\n");
	return (-1);
}

/*
int
	commandeer_pipe_sequence(t_minishell *sh, t_snode *seq_node, void *data)
{
	size_t	index;
	size_t	count;
	int		io[2];
	int		prev_out;

	index = 0;
	prev_out = STDIN_FILENO;
	count = seq_node->childs_size;
	while (index < count)
	{
		if (index < (count -1))
		{
			sh_pipe(io);
			_get_commandeer_cmd_procs()[seq_node->childs[index]->type](sh,
					seq_node->childs[index], prev_out, io[1]);
		}
		else
			_get_commandeer_cmd_procs()[seq_node->childs[index]->type](sh,
					seq_node->childs[index], prev_out, STDOUT_FILENO);
		if (prev_out != STDIN_FILENO)
			sh_close(prev_out);
		prev_out = io[0];
		index++;
	}
	return (0);
}
*/

static int
	_commandeer_pipe_sequence_iter(t_minishell *sh, t_snode  *seq_node, t_pipe_seq_ctx ctx, size_t index)
{
	pid_t				pid;
	int					io[2];
	t_snode				*cmd_node;
	const t_cmd_base	*cmd;


	cmd_node = seq_node->childs[seq_node->childs_size - index];
	cmd = &_get_commandeer_cmd_procs()[sx_simple_cmd - cmd_node->type];
	if (index == 1)
		pid = cmd->on_command(sh,
				cmd_node, ctx.prev_out_fd, STDOUT_FILENO);
	else
	{
		sh_pipe(io);
		pid = cmd->on_command(sh,
				cmd_node, ctx.prev_out_fd, io[1]);
		sh_close(io[1]);
		if (ctx.prev_out_fd != STDIN_FILENO)
			sh_close(ctx.prev_out_fd);
		ctx.prev_out_fd = io[0];
		return (_commandeer_pipe_sequence_iter(sh, seq_node, ctx, index - 1));		
	}
	if (ctx.run_in_background)
		return (0);
	return (cmd->wait(pid));
}

/* TODO disable child reaper signal handler before executing command and enabling it again afterwards */
int
	commandeer_pipe_sequence(t_minishell *sh, t_snode *seq_node, void *data)
{
	t_pipe_seq_ctx	ctx;
	int				run_in_background;

	if (seq_node->childs_size == 0)
		return (0);
	run_in_background = !!(long) data;
	ctx.run_in_background = run_in_background;
	ctx.prev_out_fd = STDIN_FILENO;
	return (_commandeer_pipe_sequence_iter(sh, seq_node, ctx, seq_node->childs_size));
}
