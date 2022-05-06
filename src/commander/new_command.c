#include "commander.h"

#include <stdlib.h>
#include <unistd.h>


#include <stdio.h>

static pid_t
	execute_external_command(const t_snode *command,
			char **argv, const int io[SH_STDIO_SIZE]);

static int
	command_should_execute(const t_snode *command)
{
	(void) command;
	if (sh()->continuing)
		return (0);
	return (1);
}

static int
	get_argument_count(char *const *argv)
{
	int	argument_count;

	argument_count = 0;
	while (*argv != NULL)
	{
		argv += 1;
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

int
	command_setup_internal_redirects(const t_snode *redirect_list,
			const int io[SH_STDIO_SIZE], int old_io[SH_STDIO_SIZE])
{
	ft_memcpy(old_io, sh()->io, sizeof(sh()->io));
	ft_memcpy(sh()->io, io, sizeof(sh()->io));
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
	command_setup_external_redirects(const t_snode *redirect_list,
			const int io[SH_STDIO_SIZE])
{
	sh_assert(redirect_list->type == sx_io_redirect_list);
	close_or_dup2_fd(io[SH_STDIN_INDEX], STDIN_FILENO);
	close_or_dup2_fd(io[SH_STDOUT_INDEX], STDOUT_FILENO);
	close_or_dup2_fd(io[SH_STDERR_INDEX], STDERR_FILENO);
	if (_cm_setup_process_redirects(redirect_list))
		return (-1);
	return (0);
}

int
	command_restore_internal_redirects(const int io[SH_STDIO_SIZE], const int old_io[SH_STDIO_SIZE])
{
	int	*shell_io;

	shell_io = sh()->io;
	if (shell_io[SH_STDIN_INDEX] != io[SH_STDIN_INDEX]
		&& shell_io[SH_STDIN_INDEX] != SH_CLOSED_FD)
		close_nostd_fd(shell_io[SH_STDIN_INDEX]);
	if (shell_io[SH_STDOUT_INDEX] != io[SH_STDOUT_INDEX]
		&& shell_io[SH_STDOUT_INDEX] != SH_CLOSED_FD)
		close_nostd_fd(shell_io[SH_STDOUT_INDEX]);
	if (shell_io[SH_STDERR_INDEX] != io[SH_STDERR_INDEX]
		&& shell_io[SH_STDERR_INDEX] != SH_CLOSED_FD)
		close_nostd_fd(shell_io[SH_STDERR_INDEX]);
	ft_memcpy(sh()->io, old_io, sizeof(sh()->io));
	return (0);
}

static pid_t
	execute_builtin(const t_snode *command, const t_builtin *function,
			char **argv, const int io[SH_STDIO_SIZE])
{
	int	return_code;
	int	argc;
	int	old_io[SH_STDIO_SIZE];

	argc = get_argument_count(argv);
	if (command_setup_internal_redirects(command->childs[1], io, old_io))
		return (SH_ERROR_INTERNAL_PID);
	return_code = function->fn(argc, argv);
	command_restore_internal_redirects(io, old_io);
	sh_env_clean();
	return (return_code_to_internal_pid(return_code));
}

static pid_t
	execute_function(const t_snode *command, const t_function *function,
			char **argv, const int io[SH_STDIO_SIZE])
{
	pid_t	internal_pid;
	char	**old_args;
	char	*old_arg0;

	(void) command;
	old_arg0 = argv[0];
	argv[0] = sh()->args[0];
	old_args = sh()->args;
	sh()->args = argv;
	internal_pid = cm_function(function->body, io);
	argv[0] = old_arg0;
	sh()->args = old_args;
	sh_env_clean();
	return (internal_pid);
}

static pid_t
	find_and_execute_builtin(const t_snode *command,
			char **argv, const int io[SH_STDIO_SIZE])
{
	size_t			index;
	size_t			size;
	const t_builtin	*builtin_func;

	index = 0;
	size = sh()->builtins_size;
	while (index < size)
	{
		builtin_func = &sh()->builtins[index];
		if (ft_strcmp(argv[0], builtin_func->key) == 0)
		{
			return (execute_builtin(command, builtin_func, argv, io));
		}
		index++;
	}
	return (SH_INVALID_INTERNAL_PID);
}

static pid_t
	find_and_execute_function(const t_snode *command,
			char **argv, const int io[SH_STDIO_SIZE])
{
	size_t		index;
	size_t		size;
	t_function	*function_func;

	index = 0;
	size = sh()->functions_size;
	while (index < size)
	{
		function_func = &sh()->functions[index];
		if (ft_strcmp(argv[0], function_func->key) == 0)
		{
			return (execute_function(command, function_func, argv, io));
		}
		index++;
	}
	return (SH_INVALID_INTERNAL_PID);
}

static int
	find_and_execute_utility(const t_snode *command,
			char **argv, const int io[SH_STDIO_SIZE])
{
	size_t			index;
	size_t			size;
	const t_builtin	*utility_func;

	index = 0;
	size = sh()->utilities_size;
	while (index < size)
	{
		utility_func = &sh()->utilities[index];
		if (ft_strcmp(argv[0], utility_func->key) == 0)
		{
			return (execute_builtin(command, utility_func, argv, io));
		}
		index++;
	}
	return (SH_INVALID_INTERNAL_PID);
}

static pid_t
	execute_internal_command(const t_snode *command,
			char **argv, const int io[SH_STDIO_SIZE])
{
	pid_t	command_pid;

	command_pid = find_and_execute_builtin(command, argv, io);
	if (command_pid != SH_INVALID_INTERNAL_PID)
		return (command_pid);
	command_pid = find_and_execute_function(command, argv, io);
	if (command_pid != SH_INVALID_INTERNAL_PID)
		return (command_pid);
	command_pid = find_and_execute_utility(command, argv, io);
	if (command_pid != SH_INVALID_INTERNAL_PID)
		return (command_pid);
	return (SH_INVALID_INTERNAL_PID);
}
/*
static pid_t
	execute_simple_command(const t_snode *command,
			char **argv, const int io[SH_STDIO_SIZE])
{
	pid_t	command_pid;

	command_pid = execute_internal_command(command, argv, io);
	if (command_pid == SH_INVALID_INTERNAL_PID)
		command_pid = execute_external_command(command, argv, io);
	return (command_pid);
}*/

static void
	setup_and_try_execve_command(const t_snode *command,
			char **argv, const int io[SH_STDIO_SIZE])
{
	if (command_setup_external_redirects(command->childs[1], io))
		exit(EXIT_FAILURE);
	sh_execvp(argv);
}

static pid_t
	execute_external_command(const t_snode *command,
			char **argv, const int io[SH_STDIO_SIZE])
{
	pid_t	command_pid;

	command_pid = sh_fork();
	if (command_pid == 0)
	{
		setup_and_try_execve_command(command, argv, io);
		perror("new command");
		exit(-1);
	}
	return (command_pid);
}

pid_t
	commandeer_simple_command(const t_snode *command,
			const int io[SH_STDIO_SIZE])
{
	pid_t	command_pid;
	char	**argv;

	if (!command_should_execute(command))
		return (return_code_to_internal_pid(0));
	argv = command_get_arguments(command);
	if (argv == NULL
		|| _do_assignments(command->childs[command->childs_size - 1],
			!!argv[0]))
		return (return_code_to_internal_pid(1));
	if (argv[0] == NULL)
		return (return_code_to_internal_pid(0));
	command_pid = execute_internal_command(command, argv, io);
	if (command_pid == SH_INVALID_INTERNAL_PID)
	{
		command_pid = execute_external_command(command, argv, io);
	}
	sh_strlst_clear(argv);
	return (command_pid);
}
