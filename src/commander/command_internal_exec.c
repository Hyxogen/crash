#include "commander.h"

pid_t
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

pid_t
	execute_function(const t_snode *command, t_snode *body,
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
	body->refcount += 1;
	internal_pid = cm_function(body, io);
	node_destroy(body);
	argv[0] = old_arg0;
	sh()->args = old_args;
	sh_env_clean();
	return (internal_pid);
}

pid_t
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

pid_t
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
			return (execute_function(command, function_func->body, argv, io));
		}
		index++;
	}
	return (SH_INVALID_INTERNAL_PID);
}

pid_t
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
