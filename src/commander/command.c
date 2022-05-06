#include "commander.h"

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

static pid_t
	execute_external_command(const t_snode *command,
		char **argv, const int io[SH_STDIO_SIZE]);

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
		handle_execvp_error(argv[0], errno);
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
