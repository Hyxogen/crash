#include "commander.h"

static int
	command_is_internal(pid_t command_pid)
{
	if (command_pid <= 0)
		return (1);
	return (0);
}

static int
	internal_wait_and_get_return_code(pid_t command_pid)
{
	return (internal_pid_to_return_code(command_pid));
}

int
	process_wait_and_get_return_code(pid_t pid)
{
	int	status_code;

	sh_waitpid(pid, &status_code, WUNTRACED);
	return (status_code_to_return_code(status_code));
}

int
	wait_and_get_return_code(pid_t command_pid)
{
	if (command_is_internal(command_pid))
		return (internal_wait_and_get_return_code(command_pid));
	return (process_wait_and_get_return_code(command_pid));
}