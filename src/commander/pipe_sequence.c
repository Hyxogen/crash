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

char **_array_add(char **array, char *value)
{
	size_t	i;

	i = 0;
	while (array[i] != NULL)
		i += 1;
	array = sh_safe_realloc(array, sizeof(*array) * (i + 1), sizeof(*array) * (i + 2));
	array[i] = value;
	array[i + 1] = NULL;
	return (array);
}

char **_word_list_to_array(t_minishell *sh, t_snode *word_list)
{
	char	**ret;
	char	**tmp;
	size_t	i;
	size_t	j;

	sh_assert(word_list->type == sx_wordlist);
	ret = sh_safe_malloc(sizeof(*ret));
	ret[0] = NULL;
	i = 0;
	while (i < word_list->childs_size)
	{
		tmp = cm_expand(sh, &word_list->childs[i]->token);
		j = 0;
		while (tmp[j] != NULL)
		{
			ret = _array_add(ret, tmp[j]);
			j += 1;
		}
		free(tmp);
		i += 1;
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

static pid_t
	_cm_simple_builtin_cmd(t_simple_cmd_ctx *ctx, t_builtin_proc proc)
{
	(void) ctx;
	(void) proc;
	/* TODO execute builtin */
	return (0);
}

static pid_t
	_cm_simple_extern_cmd(t_simple_cmd_ctx *ctx)
{
	pid_t	pid;
	
	pid = sh_fork();
	if (pid == 0)
	{
		dup2(ctx->fd_in, STDIN_FILENO);
		dup2(ctx->fd_out, STDOUT_FILENO);
		_cm_setup_redirects(ctx->sh, ctx->cmd_node->childs[1]);
		if (ctx->fd_in != STDIN_FILENO)
			close(ctx->fd_in);
		if (ctx->fd_out != STDOUT_FILENO)
			close(ctx->fd_out);
		sh_execvp(ctx->sh, ctx->args);
		_sh_execvp_error_handler(ctx->args[0], errno);
	}
	return (pid);
}

pid_t
	cm_simple_cmd_command(t_minishell *sh, t_snode *cmd_node, int fd_in, int fd_out)
{
	size_t				i;
	t_simple_cmd_ctx	ctx;

	ctx.args = _word_list_to_array(sh, cmd_node->childs[0]);
	ctx.fd_in = fd_in;
	ctx.fd_out = fd_out;
	ctx.cmd_node = cmd_node;
	ctx.sh = sh;
	i = 0;
	while (i < sh->builtins_size)
	{
		if (!ft_strcmp(ctx.args[0], sh->builtins[i].key))
			return (_cm_simple_builtin_cmd(&ctx, sh->builtins[i].fn));
		i += 1;
	}
	return (_cm_simple_extern_cmd(&ctx));
}

int
	cm_simple_cmd_wait(pid_t pid)
{
	int		status;

	sh_assert(waitpid(pid, &status, WUNTRACED) > 0);
	return (_get_exit_code(status));
}

static t_cm_cmd_proc
	*_get_commandeer_cmd_procs(void)
{
	static const t_cm_cmd_proc procs[] = {
		cm_simple_cmd_command,
		cm_unimplemented_cmd_command,
		cm_unimplemented_cmd_command,
		cm_unimplemented_cmd_command,
		cm_unimplemented_cmd_command,
		cm_unimplemented_cmd_command,
		cm_unimplemented_cmd_command
	};

	return (procs);
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

static int
	_commandeer_pipe_sequence_iter(t_minishell *sh, t_snode  *seq_node, int prev_out_fd, size_t index)
{
	pid_t				pid;
	int					io[2];
	t_snode				*cmd_node;
	int					exit_code;

	cmd_node = seq_node->childs[seq_node->childs_size - index];
	if (index == 1)
		pid = _get_commandeer_cmd_procs()[sx_simple_cmd - cmd_node->type](sh,
				cmd_node, prev_out_fd, STDOUT_FILENO);
	else
	{
		sh_pipe(io);
		pid = _get_commandeer_cmd_procs()[sx_simple_cmd - cmd_node->type](sh,
				cmd_node, prev_out_fd, io[1]);
		sh_close(io[1]);
		if (prev_out_fd != STDIN_FILENO)
			sh_close(prev_out_fd);
		prev_out_fd = io[0];
		exit_code = _commandeer_pipe_sequence_iter(sh, seq_node, prev_out_fd, index - 1);
		sh_waitpid(pid, NULL, WUNTRACED);
		return (exit_code);
	}
	sh_waitpid(pid, &exit_code, 0);
	return (_get_exit_code(exit_code));
}

/* TODO fork the entire pipe sequence when it runs in the background */
int
	commandeer_pipe_sequence(t_minishell *sh, t_snode *seq_node, void *data)
{
	int				run_in_background;
	int				rc;

	if (seq_node->childs_size == 0)
		return (0);
	run_in_background = !!(long) data;
	cm_disable_reaper(sh);
	rc = _commandeer_pipe_sequence_iter(sh, seq_node, STDIN_FILENO, seq_node->childs_size);
	cm_enable_reaper(sh);
	return (rc);
}
