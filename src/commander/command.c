#include "commander.h"

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


#include <stdio.h>

static void
	_do_assignments(t_snode *ass_list, int is_tmp)
{
	size_t	i;
	size_t	j;
	char	*tmp;

	i = 0;
	while (i < ass_list->childs_size)
	{
		j = 0;
		while (ass_list->childs[i]->token.str[j] != '=')
			j += 1;
		tmp = cm_expand_str(&ass_list->childs[i]->token, NULL, ' ');
		if (tmp != NULL)
		{
			tmp[j] = '\0';
			sh_setenv(tmp, tmp + j + 1, is_tmp);
			free(tmp);
		}
		i += 1;
	}
}

static void
	_sh_execvp_error_handler(const char *name, int error)
{
	if (error == ENOENT)
	{
		sh_err2(name, "command not found");
		exit(127);
	}
	else if (error == EACCES)
	{
		sh_err2(name, "permission denied");
		exit(127);
	}
	else
	{
		sh_err2(name, "unknown error");
		sh_abort();
	}   
}

static pid_t
	_cm_simple_builtin_cmd(t_simple_cmd_ctx *ctx, t_builtin_proc proc)
{
	int	rc;
	int	argc;
	int	io_cpy[3];

	argc = 0;
	ft_memcpy(io_cpy, sh()->io, sizeof(io_cpy));
	ft_memcpy(sh()->io, ctx->io, sizeof(io_cpy));
	while (ctx->args[argc] != NULL)
		argc += 1;
	if (_cm_setup_builtin_redirects(ctx->cmd_node->childs[1], sh()->io))
		return (cm_convert_retcode(-1));
	rc = proc(argc, ctx->args);
	if (sh()->io[SH_STDIN_INDEX] != ctx->io[SH_STDIN_INDEX])
		_cm_close_nostd(sh()->io[SH_STDIN_INDEX]);
	if (sh()->io[SH_STDOUT_INDEX] != ctx->io[SH_STDOUT_INDEX])
		_cm_close_nostd(sh()->io[1]);
	if (sh()->io[SH_STDERR_INDEX] != ctx->io[SH_STDERR_INDEX])
		_cm_close_nostd(sh()->io[SH_STDERR_INDEX]);
	ft_memcpy(sh()->io, io_cpy, sizeof(io_cpy));
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
		_cm_setup_process_redirects(ctx->cmd_node->childs[1]);
		if (ctx->io[SH_STDIN_INDEX] != STDIN_FILENO)
			sh_close(ctx->io[SH_STDIN_INDEX]);
		if (ctx->io[SH_STDOUT_INDEX] != STDOUT_FILENO)
			sh_close(ctx->io[SH_STDOUT_INDEX]);
		if (ctx->io[SH_STDERR_INDEX] != STDERR_FILENO)
			sh_close(ctx->io[SH_STDERR_INDEX]);
		sh_execvp(ctx->args);
		_sh_execvp_error_handler(ctx->args[0], errno);
	}
	return (pid);
}

pid_t
	cm_simple_cmd_command(t_snode *cmd_node, const int io[3], int closefd)
{
	size_t				i;
	t_simple_cmd_ctx	ctx;
	pid_t				ret;

	ctx.args = cm_word_list_to_array(cmd_node->childs[0]);
	_do_assignments(cmd_node->childs[2], !!ctx.args[0]);
	if (!ctx.args[0])
		return (-1); /* TODO Should this be handled differenlty? */
	ctx.io[SH_STDIN_INDEX] = io[SH_STDIN_INDEX];
	ctx.io[SH_STDOUT_INDEX] = io[SH_STDOUT_INDEX];
	ctx.io[SH_STDERR_INDEX] = io[SH_STDERR_INDEX];
	ctx.cmd_node = cmd_node;
	ctx.closefd = closefd;
	i = 0;
	while (i < sh()->builtins_size)
	{
		if (!ft_strcmp(ctx.args[0], sh()->builtins[i].key))
		{
			ret = _cm_simple_builtin_cmd(&ctx, sh()->builtins[i].fn);
			sh_env_clean();
			return (ret);
		}
		i += 1;
	}
	ret = _cm_simple_extern_cmd(&ctx);
	sh_env_clean();
	return (ret);
}
