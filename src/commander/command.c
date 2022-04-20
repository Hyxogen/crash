#include "commander.h"
#include "minishell.h"

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


#include <stdio.h>

static int
	_do_assignments(const t_snode *ass_list, int is_tmp)
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

/* Closes the tofd if the fromfd is equal to -1. Otherwise it will dup2(formfd, tofd) */
static void
	_cm_close_or_dup2(int fromfd, int tofd)
{
	if (fromfd == -1)
		sh_close(tofd);
	else
		sh_dup2(fromfd, tofd);
}

/* TODO Test something like closing a fd and then reopening that fd <&- <file.txt */
static pid_t
	_cm_simple_extern_cmd(t_simple_cmd_ctx *ctx)
{
	pid_t	pid;

	pid = sh_fork();
	if (pid == 0)
	{
		_cm_close_or_dup2(ctx->io[SH_STDIN_INDEX], STDIN_FILENO);
		_cm_close_or_dup2(ctx->io[SH_STDOUT_INDEX], STDOUT_FILENO);
		_cm_close_or_dup2(ctx->io[SH_STDERR_INDEX], STDERR_FILENO);
		if (_cm_setup_process_redirects(ctx->cmd_node->childs[1]))
			exit(1);
		if (ctx->io[SH_STDIN_INDEX]
			!= STDIN_FILENO && ctx->io[SH_STDIN_INDEX] >= 0)
			sh_close(ctx->io[SH_STDIN_INDEX]);
		if (ctx->io[SH_STDOUT_INDEX]
			!= STDOUT_FILENO && ctx->io[SH_STDOUT_INDEX] >= 0)
			sh_close(ctx->io[SH_STDOUT_INDEX]);
		if (ctx->io[SH_STDERR_INDEX]
			!= STDERR_FILENO && ctx->io[SH_STDERR_INDEX] >= 0)
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
	char	**old_args;
	char	*old_arg0;

	index = 0;
	size = sh()->functions_size;
	while (index < size)
	{
		if (!ft_strcmp(ctx->args[0], sh()->functions[index].key))
		{
			old_arg0 = ctx->args[0];
			ctx->args[0] = sh()->args[0];
			old_args = sh()->args;
			sh()->args = ctx->args;
			ret = cm_function(sh()->functions[index].body, ctx->io);
			ctx->args[0] = old_arg0;
			sh()->args = old_args;
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
	cm_simple_cmd_command(const t_snode *cmd_node, const int io[3])
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

/* NOTE: Ik was bezig met het refactoren van deze file, maar het nieuwe systeem werkt nog niet helemaal
 * for the time being heb ik even het oude systeem nog aan laten staan */

static int
	command_should_execute(const t_snode *command)
{
	(void) command;
	if (sh()->continuing)
		return (0);
	return (1);
}

static int
	get_argument_count(char * const *args)
{
	int	argument_count;

	argument_count = 0;
	while (*args != NULL)
	{
		args += 1;
		argument_count += 1;
	}
	return (argument_count);
}

static char
	**command_get_arguments(const t_snode *command)
{
	return (cm_word_list_to_array(command->childs[0]));
}

static int
	close_nostd_fd(int fd)
{
	if (fd != STDIN_FILENO && fd != STDOUT_FILENO && fd != STDERR_FILENO)
		return (sh_close(fd));
	return (0);
}

static int
	command_setup_internal_redirects(const t_snode *command, const int io[SH_STDIO_SIZE], int old_io[SH_STDIO_SIZE])
{
	const t_snode	*redirect_list;

	ft_memcpy(old_io, sh()->io, sizeof(sh()->io));
	ft_memcpy(sh()->io, io, sizeof(sh()->io));
	redirect_list = command->childs[1];
	if (_cm_setup_builtin_redirects(redirect_list, sh()->io))
		return (-1);
	return (0);
}

static void
	close_or_dup2_fd(int fromfd, int tofd)
{
	if (fromfd == SH_CLOSED_FD)
		sh_close(tofd);
	else
		sh_dup2(fromfd, tofd);
}

static int
	command_setup_external_redirects(const t_snode *command, const int io[SH_STDIO_SIZE])
{
	const t_snode	*redirect_list;

	redirect_list = command->childs[1];
	close_or_dup2_fd(io[SH_STDIN_INDEX], STDIN_FILENO);
	close_or_dup2_fd(io[SH_STDOUT_INDEX], STDOUT_FILENO);
	close_or_dup2_fd(io[SH_STDERR_INDEX], STDERR_FILENO);
	if (_cm_setup_process_redirects(redirect_list))
		return (-1);
	return (0);
}

static int
	command_restore_internal_redirects(const t_snode *command, const int io[SH_STDIO_SIZE])
{
	int	*shell_io;

	(void) command;
	shell_io = sh()->io;
	if (shell_io[SH_STDIN_INDEX] != io[SH_STDIN_INDEX] && shell_io[SH_STDIN_INDEX] != SH_CLOSED_FD)
		close_nostd_fd(shell_io[SH_STDIN_INDEX]);
	if (shell_io[SH_STDOUT_INDEX] != io[SH_STDOUT_INDEX] && shell_io[SH_STDOUT_INDEX] != SH_CLOSED_FD)
		close_nostd_fd(shell_io[SH_STDOUT_INDEX]);
	if (shell_io[SH_STDERR_INDEX] != io[SH_STDERR_INDEX] && shell_io[SH_STDERR_INDEX] != SH_CLOSED_FD)
		close_nostd_fd(shell_io[SH_STDERR_INDEX]);
	return (0);
}

static pid_t
	execute_internal_function(const t_snode *command, const t_builtin *function, char **args, const int io[SH_STDIO_SIZE])
{
	int	return_code;
	int	argc;
	int	old_io[SH_STDIO_SIZE];

	argc = get_argument_count(args);
	if (command_setup_internal_redirects(command, io, old_io))
		return (SH_ERROR_INTERNAL_PID);
	return_code = function->fn(argc, args);
	command_restore_internal_redirects(command, old_io);
	sh_env_clean();
	return (return_code_to_internal_pid(return_code));
}

static pid_t
	execute_defined_function(const t_snode *command, t_function *function, char **args, const int io[SH_STDIO_SIZE])
{
	int		return_code;
	char	**old_args;
	char	*old_arg0;

	(void) command;
	old_arg0 = args[0];
	args[0] = sh()->args[0];
	old_args = sh()->args;
	sh()->args = args;
	return_code = cm_function(function->body, io);
	args[0] = old_arg0;
	sh()->args = old_args;
	sh_env_clean();
	return (return_code_to_internal_pid(return_code));
}

static pid_t
	find_and_execute_builtin(const t_snode *command, char **args, const int io[SH_STDIO_SIZE])
{
	size_t			index;
	size_t			size;
	const t_builtin	*builtin_func;

	index = 0;
	size = sh()->builtins_size;
	while (index < size)
	{
		builtin_func = &sh()->builtins[index];
		if (ft_strcmp(args[0], builtin_func->key) == 0)
		{
			return (execute_internal_function(command, builtin_func, args, io));
		}
		index++;
	}
	return (SH_INVALID_INTERNAL_PID);
}

static pid_t
	find_and_execute_utility(const t_snode *command, char **args, const int io[SH_STDIO_SIZE])
{
	size_t			index;
	size_t			size;
	const t_builtin	*utility_func;

	index = 0;
	size = sh()->utilities_size;;
	while (index < size)
	{
		utility_func = &sh()->utilities[index];
		if (ft_strcmp(args[0], utility_func->key) == 0)
		{
			return (execute_internal_function(command, utility_func, args, io));
		}
		index++;
	}
	return (SH_INVALID_INTERNAL_PID);
}

static pid_t
	find_and_execute_defined_function(const t_snode *command, char **args, const int io[SH_STDIO_SIZE])
{
	size_t		index;
	size_t		size;
	t_function	*function_func;

	index = 0;
	size = sh()->functions_size;
	while (index < size)
	{
		function_func = &sh()->functions[index];
		if (ft_strcmp(args[0], function_func->key) == 0)
		{
			return (execute_defined_function(command, function_func, args, io));
		}
		index++;
	}
	return (SH_INVALID_INTERNAL_PID);
}

static pid_t
	execute_internal_command(const t_snode *command, char **args, const int io[SH_STDIO_SIZE])
{
	pid_t	command_pid;

	command_pid = find_and_execute_builtin(command, args, io);
	if (command_pid != SH_INVALID_INTERNAL_PID)
		return (command_pid);
	command_pid = find_and_execute_defined_function(command, args, io);
	if (command_pid != SH_INVALID_INTERNAL_PID)
		return (command_pid);
	command_pid = find_and_execute_utility(command, args, io);
	if (command_pid != SH_INVALID_INTERNAL_PID)
		return (command_pid);
	return (SH_INVALID_INTERNAL_PID);
}

static void
	setup_and_try_execve_command(const t_snode *command, char **args, const int io[SH_STDIO_SIZE])
{
	if (command_setup_external_redirects(command, io))
		exit(EXIT_FAILURE);
	sh_execvp(args);
}

static pid_t
	execute_external_command(const t_snode *command, char **args, const int io[SH_STDIO_SIZE])
{
	pid_t	command_pid;

	command_pid = sh_fork();
	if (command_pid == 0)
	{
		setup_and_try_execve_command(command, args, io);
		sh_assert(0);
	}
	return (command_pid);
}

pid_t
	execute_simple_command(const t_snode *command, const int io[SH_STDIO_SIZE])
{
	pid_t	command_pid;
	char	**args;

	if (!command_should_execute(command))
		return (return_code_to_internal_pid(0));
	args = command_get_arguments(command);
	if (args == NULL)
		return (return_code_to_internal_pid(1));
	if (args[0] == NULL)
		return (return_code_to_internal_pid(0));
	command_pid = execute_internal_command(command, args, io);
	if (command_pid == SH_INVALID_INTERNAL_PID)
	{
		command_pid = execute_external_command(command, args, io);
	}
	sh_strlst_clear(args);
	return (command_pid);
}
