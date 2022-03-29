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
	_cm_simple_builtin_cmd(t_minishell *sh, t_simple_cmd_ctx *ctx, t_builtin_proc proc)
{
	/* TODO execute builtin */
	/*
	while (i < sh->builtins_size)
	{
		if (ft_strcmp(argv[0], sh->builtins[i].key) == 0)
			return (sh->builtins[i].fn(sh, argv), 0);
		i += 1;
	}
	*/
	proc(sh, ctx->args, ctx->io);
	return (0);
}

static pid_t
	_cm_simple_extern_cmd(t_simple_cmd_ctx *ctx)
{
	pid_t	pid;
	
	pid = sh_fork();
	if (pid == 0)
	{
		sh_dup2(ctx->io[SH_STDIN_INDEX], STDIN_FILENO);
		sh_dup2(ctx->io[SH_STDOUT_INDEX], STDOUT_FILENO);
		sh_dup2(ctx->io[SH_STDERR_INDEX], STDERR_FILENO);
		_cm_setup_redirects(ctx->sh, ctx->cmd_node->childs[1]);
		if (ctx->io[SH_STDIN_INDEX] != STDIN_FILENO)
			sh_close(ctx->io[SH_STDIN_INDEX]);
		if (ctx->io[SH_STDOUT_INDEX] != STDOUT_FILENO)
			sh_close(ctx->io[SH_STDOUT_INDEX]);
		if (ctx->io[SH_STDERR_INDEX] != STDERR_FILENO)
			sh_close(ctx->io[SH_STDERR_INDEX]);
		sh_execvp(ctx->sh, ctx->args);
		_sh_execvp_error_handler(ctx->args[0], errno);
	}
	return (pid);
}

pid_t
	cm_simple_cmd_command(t_minishell *sh, t_snode *cmd_node, const int io[3])
{
	size_t				i;
	t_simple_cmd_ctx	ctx;

	ctx.args = _word_list_to_array(sh, cmd_node->childs[0]);
	ctx.io[SH_STDIN_INDEX] = io[SH_STDIN_INDEX];
	ctx.io[SH_STDOUT_INDEX] = io[SH_STDOUT_INDEX];
	ctx.io[SH_STDERR_INDEX] = io[SH_STDERR_INDEX]; // TODO
	ctx.cmd_node = cmd_node;
	ctx.sh = sh;
	i = 0;
	while (i < sh->builtins_size)
	{
		if (!ft_strcmp(ctx.args[0], sh->builtins[i].key))
			return (_cm_simple_builtin_cmd(sh, &ctx, sh->builtins[i].fn));
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
	static t_cm_cmd_proc procs[] = {
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
	cm_unimplemented_cmd_command(t_minishell *sh, t_snode *node, const int io[3])
{
	(void) sh;
	(void) node;
	(void) io;

	fprintf(stderr, "Executing this command type is not implemented yet\n");
	return (-1);
}

static void
	_cm_close_nostd(int fd)
{
	if (fd != STDIN_FILENO && fd != STDOUT_FILENO && fd != STDERR_FILENO)
		sh_close(fd);
}

static int
	_commandeer_pipe_sequence_rec(t_minishell *sh, const t_pipe_ctx *ctx, int prev_out_fd, size_t index)
{
	pid_t			pid;
	int				cmd_io[3];
	int				pipe_io[2];
	t_snode			*cmd_node;
	t_cm_cmd_proc	proc;
	int				ex_code;

	ft_memcpy(cmd_io, ctx->io, sizeof(int) * 3);
	cmd_node = ctx->pipe_node->childs[ctx->pipe_node->childs_size - index];
	proc = _get_commandeer_cmd_procs()[sx_simple_cmd - cmd_node->type];
	if (index != 1)
	{
		sh_pipe(pipe_io);
		cmd_io[SH_STDOUT_INDEX] = pipe_io[1];
		cmd_io[SH_STDIN_INDEX] = prev_out_fd;
		pid = proc(sh, cmd_node, cmd_io); /* TODO make sure pipe_io[0] fd is closed in child */
		_cm_close_nostd(prev_out_fd);
		ex_code = _commandeer_pipe_sequence_rec(sh, ctx, pipe_io[0], index - 1);
		sh_waitpid(pid, NULL, 0);
		return (ex_code);
	}
	pid = proc(sh, cmd_node, cmd_io);
	_cm_close_nostd(cmd_io[STDIN_FILENO]);
	waitpid(pid, &ex_code, 0);
	return (_get_exit_code(ex_code));
}


/* TODO fork the entire pipe sequence when it runs in the background */
int
	commandeer_pipe_sequence(t_minishell *sh, t_snode *seq_node, const int io[3])
{
	t_pipe_ctx	ctx;
	int			rc;

	if (seq_node->childs_size == 0)
		return (0);
	ctx.pipe_node = seq_node;
	ft_memcpy(ctx.io, io, sizeof(int) * 3);
	cm_disable_reaper(sh);
	rc = _commandeer_pipe_sequence_rec(sh, &ctx, io[STDIN_FILENO], seq_node->childs_size);
	cm_enable_reaper(sh);
	return (rc);
}
