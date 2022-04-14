#include "commander.h"
#include "minishell.h"

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


#include <stdio.h>

static int
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
		if (tmp == NULL)
		{
			sh_env_clean();
			return (-1);
		}
		tmp[j] = '\0';
		sh_setenv(tmp, tmp + j + 1, is_tmp);
		free(tmp);
		i += 1;
	}
	return (0);	
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
		sh_err3(name, "unknown error", strerror(errno));
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
		sh_dup2(ctx->io[SH_STDIN_INDEX], STDIN_FILENO);
		sh_dup2(ctx->io[SH_STDOUT_INDEX], STDOUT_FILENO);
		sh_dup2(ctx->io[SH_STDERR_INDEX], STDERR_FILENO);
		if (_cm_setup_process_redirects(ctx->cmd_node->childs[1]))
			exit(1);
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

/* Returns pid > 0 if none found */
static pid_t
	_cm_builtin(t_simple_cmd_ctx *ctx)
{
	size_t	index;
	size_t	size;
	pid_t	ret;

	index = 0;
	size = sh()->builtins_size;
	while (index < size)
	{
		if (!ft_strcmp(ctx->args[0], sh()->builtins[index].key))
		{
			ret = _cm_simple_builtin_cmd(ctx, sh()->builtins[index].fn);
			sh_env_clean();
			sh_strlst_clear(ctx->args);
			return (ret);
		}
		index++;
	}
	return (1);
}

/* Returns pid > 0 if none found */
static pid_t
	_cm_function(t_simple_cmd_ctx *ctx)
{
	size_t	index;
	size_t	size;
	pid_t	ret;

	index = 0;
	size = sh()->functions_size;
	while (index < size)
	{
		if (!ft_strcmp(ctx->args[0], sh()->functions[index].key))
		{
			ret = cm_function(sh()->functions[index].body, ctx->io);
			sh_env_clean();
			sh_strlst_clear(ctx->args);
			return (ret);
		}
		index++;
	}
	return (1);
}

/* Returns pid > 0 if none found */
static pid_t
	_cm_utility(t_simple_cmd_ctx *ctx)
{
	size_t	index;
	size_t	size;
	pid_t	ret;

	index = 0;
	size = sh()->utilities_size;
	while (index < size)
	{
		if (!ft_strcmp(ctx->args[0], sh()->utilities[index].key))
		{
			ret = _cm_simple_builtin_cmd(ctx, sh()->utilities[index].fn);
			sh_env_clean();
			sh_strlst_clear(ctx->args);
			return (ret);
		}
		index++;
	}
	return (1);
}

/* Returns pid > 0 if none found */
static pid_t
	_cm_internal(t_simple_cmd_ctx *ctx)
{
	pid_t	pid;

	pid = _cm_builtin(ctx);
	if (pid <= 0)
		return (pid);
	pid = _cm_function(ctx);
	if (pid <= 0)
		return (pid);
	pid = _cm_utility(ctx);
	return (pid);
}

pid_t
	cm_simple_cmd_command(t_snode *cmd_node, const int io[3])
{
	t_simple_cmd_ctx	ctx;
	pid_t				ret;

	if (sh()->continuing)
		return (cm_convert_retcode(0));
	ctx.args = cm_word_list_to_array(cmd_node->childs[0]);
	if (ctx.args == 0 || _do_assignments(cmd_node->childs[2], !!ctx.args[0]))
		return (sh_strlst_clear(ctx.args), cm_convert_retcode(1));
	if (!ctx.args[0])
		return (sh_strlst_clear(ctx.args), cm_convert_retcode(0));
	ft_memcpy(ctx.io, io, sizeof(ctx.io));
	ctx.cmd_node = cmd_node;
	ret = _cm_internal(&ctx);
	if (ret <= 0)
		return (ret);
	ret = _cm_simple_extern_cmd(&ctx);
	sh_env_clean();
	sh_strlst_clear(ctx.args);
	return (ret);
}

/* TODO move this to a different file */
void
	cm_close_nstd_nred(const int original[3], const int redirect[3])
{
	if (original[SH_STDIN_INDEX] != redirect[SH_STDIN_INDEX])
		_cm_close_nostd(redirect[SH_STDIN_INDEX]);
	if (original[SH_STDOUT_INDEX] != redirect[SH_STDOUT_INDEX])
		_cm_close_nostd(redirect[SH_STDOUT_INDEX]);
	if (original[SH_STDERR_INDEX] != redirect[SH_STDERR_INDEX])
		_cm_close_nostd(redirect[SH_STDERR_INDEX]);
}
