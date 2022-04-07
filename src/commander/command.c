#include "commander.h"

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>


#include <stdio.h>

static void
	_sh_execvp_error_handler(const char *name, int error)
{
	if (error == ENOENT)
	{
		fprintf(stderr, "CraSH: Could not find executable: \"%s\"\n", name);
		exit(127);
	}
	else if (error == EACCES)
	{
		fprintf(stderr, "CraSH: \"%s\" permission denied\n", name);
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
	int	rc;
	int	argc;

	argc = 0;
	while (ctx->args[argc] != NULL)
		argc += 1;
	_cm_setup_builtin_redirects(sh, ctx->cmd_node->childs[1], ctx->io);
	rc = proc(sh, argc, ctx->args, ctx->io);
	_cm_close_nostd(ctx->io[SH_STDIN_INDEX]);
	_cm_close_nostd(ctx->io[SH_STDOUT_INDEX]);
	_cm_close_nostd(ctx->io[SH_STDERR_INDEX]);
	return (cm_convert_retcode(rc));
}

static pid_t
	_cm_simple_extern_cmd(t_simple_cmd_ctx *ctx)
{
	pid_t	pid;
	
	pid = sh_fork();
	if (pid == 0)
	{
		if (ctx->closefd >= 0)
			sh_close(ctx->closefd);
		sh_dup2(ctx->io[SH_STDIN_INDEX], STDIN_FILENO);
		sh_dup2(ctx->io[SH_STDOUT_INDEX], STDOUT_FILENO);
		sh_dup2(ctx->io[SH_STDERR_INDEX], STDERR_FILENO);
		_cm_setup_process_redirects(ctx->sh, ctx->cmd_node->childs[1]);
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
	cm_simple_cmd_command(t_minishell *sh, t_snode *cmd_node, const int io[3], int closefd)
{
	size_t				i;
	t_simple_cmd_ctx	ctx;
	pid_t				ret;

	ctx.args = cm_word_list_to_array(sh, cmd_node->childs[0]);
	_do_assignments(sh, cmd_node->childs[2], !!ctx.args[0]);
	if (!ctx.args[0])
		return (-1); /* TODO Should this be handled differenlty? */
	ctx.io[SH_STDIN_INDEX] = io[SH_STDIN_INDEX];
	ctx.io[SH_STDOUT_INDEX] = io[SH_STDOUT_INDEX];
	ctx.io[SH_STDERR_INDEX] = io[SH_STDERR_INDEX];
	ctx.cmd_node = cmd_node;
	ctx.sh = sh;
	ctx.closefd = closefd;
	i = 0;
	while (i < sh->builtins_size)
	{
		if (!ft_strcmp(ctx.args[0], sh->builtins[i].key))
		{
			ret = _cm_simple_builtin_cmd(sh, &ctx, sh->builtins[i].fn);
			sh_env_clean(sh);
			return (ret);
		}
		i += 1;
	}
	ret = _cm_simple_extern_cmd(&ctx);
	sh_env_clean(sh);
	return (ret);
}
