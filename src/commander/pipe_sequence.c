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
#include <fcntl.h>
#include <limits.h>

void
	_cm_close_nostd(int fd)
{
	if (fd >= 0 && fd != STDIN_FILENO && fd != STDOUT_FILENO && fd != STDERR_FILENO)
		sh_close(fd);
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

char **cm_word_list_to_array(const t_snode *word_list)
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
		tmp = cm_expand(&word_list->childs[i]->token);
		j = 0;
		if (tmp == NULL)
		{
			sh_strlst_clear(ret);
			return (NULL);
		}
		while (tmp[j] != NULL)
			ret = _array_add(ret, tmp[j++]);
		free(tmp);
		i += 1;
	}
	return (ret);
}

pid_t
	cm_unimplemented_cmd_command(const t_snode *node, const int io[3])
{
	(void) node;
	(void) io;

	sh_err1("executing this command type is not implemented yet");
	return (cm_convert_retcode(0));
}

#define SH_RETCODE_SIGNALLED_OFFSET 128

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

static int
	process_wait_and_get_return_code(pid_t pid)
{
	int	status_code;

	sh_waitpid(pid, &status_code, WUNTRACED);
	return (status_code_to_return_code(status_code));
}

static int
	wait_and_get_return_code(pid_t command_pid)
{
	if (command_is_internal(command_pid))
		return (internal_wait_and_get_return_code(command_pid));
	return (process_wait_and_get_return_code(command_pid));
}

static t_command
	get_command_function(const t_snode *command)
{
	static t_command functions[] = {
		cm_simple_cmd_command,
		cm_if_clause,
		cm_function_define,
		cm_case_clause,
		cm_for_clause,
		cm_while_until_clause
	};

	return (functions[command->type - sx_simple_cmd]);
}

static int
	pipe_seq_should_execute(const t_snode *pipe_seq)
{
	if (pipe_seq->childs_size == 0 || sh()->breaking > 0)
		return (0);
	return (1);
}

static pid_t
	execute_command_fork(const t_snode *command, const int io[SH_STDIO_SIZE])
{
	pid_t		command_pid;
	pid_t		fork_pid;
	t_command	command_func;

	fork_pid = sh_fork();
	if (fork_pid == 0)
	{
		command_func = get_command_function(command);
		command_pid = command_func(command, io);
		exit(wait_and_get_return_code(command_pid));
	}
	return (fork_pid);
}

static pid_t
	execute_command_nofork(const t_snode *command, const int io[SH_STDIO_SIZE])
{
	pid_t		command_pid;
	t_command	command_func;

		command_func = get_command_function(command);
		command_pid = command_func(command, io);
	return (command_pid);
}

static int
	create_pipe_and_execute_command(const t_snode *command, pid_t *command_pid, int input_fd)
{
	int	pipe_io[2];
	int	command_io[SH_STDIO_SIZE];

	sh_pipe(pipe_io);
	sh_fdctl(pipe_io[0], SH_FD_FIOCLEX, 1);
	sh_fdctl(pipe_io[1], SH_FD_FIOCLEX, 1);
	command_io[SH_STDIN_INDEX] = input_fd;
	command_io[SH_STDOUT_INDEX] = pipe_io[1];
	command_io[SH_STDERR_INDEX] = STDERR_FILENO;
	*command_pid = execute_command_fork(command, command_io);
	sh_close(pipe_io[1]);
	return (pipe_io[0]);
}

static int
	execute_commands_recursive(const t_snode *pipe_seq, const int final_out_fd, int previous_out_fd, size_t index);

static int
	execute_commands_recursive_not_last(const t_snode *pipe_seq, const int final_out_fd, int previous_out_fd, size_t index)
{
	pid_t	command_pid;
	int		last_return_code;
	int		current_out_fd;

	current_out_fd = create_pipe_and_execute_command(pipe_seq->childs[index], &command_pid, previous_out_fd);
	last_return_code = execute_commands_recursive(pipe_seq, final_out_fd, current_out_fd, index + 1);
	wait_and_get_return_code(command_pid);
	return (last_return_code);
}

static int
	execute_commands_recursive_last(const t_snode *pipe_seq, const int final_out_fd, int previous_out_fd, size_t index)
{
	pid_t	command_pid;
	int		return_code;
	int		command_io[SH_STDIO_SIZE];

	command_io[SH_STDIN_INDEX] = previous_out_fd;
	command_io[SH_STDOUT_INDEX] = final_out_fd;
	command_io[SH_STDERR_INDEX] = STDERR_FILENO;
	command_pid = execute_command_fork(pipe_seq->childs[index], command_io);
	return_code = wait_and_get_return_code(command_pid);
	return (return_code);
}

static int
	execute_commands_recursive(const t_snode *pipe_seq, const int final_out_fd, int previous_out_fd, size_t index)
{
	if (index + 1 == pipe_seq->childs_size)
		return (execute_commands_recursive_last(pipe_seq, final_out_fd, previous_out_fd, index));
	else
		return (execute_commands_recursive_not_last(pipe_seq, final_out_fd, previous_out_fd, index));
}

static int
	execute_pipe_seq_nofork(const t_snode *pipe_seq, const int io[SH_STDIO_SIZE])
{
	return (execute_commands_recursive(pipe_seq, io[SH_STDOUT_INDEX], io[SH_STDIN_INDEX], 0));
}

static int
	execute_multi_command_pipe_seq(const t_snode *pipe_seq, const int io[SH_STDIO_SIZE])
{
	pid_t	pipe_pid;
	int		return_code;

	pipe_pid = sh_fork();
	if (pipe_pid == 0)
	{
		return_code = execute_pipe_seq_nofork(pipe_seq, io);
		exit(return_code);
	}
	return_code = process_wait_and_get_return_code(pipe_pid);
	return (return_code);
}

static int
	execute_single_command_pipe_seq(const t_snode *pipe_seq, const int io[SH_STDIO_SIZE])
{
	pid_t	command_pid;
	int		return_code;

	command_pid = execute_command_nofork(pipe_seq->childs[0], io);
	return_code = wait_and_get_return_code(command_pid);
	return (return_code);
}

int
	execute_pipe_seq(const t_snode *pipe_seq, const int io[SH_STDIO_SIZE])
{
	int	return_code;

	if (!pipe_seq_should_execute(pipe_seq))
		return (0);
	child_reaper_lock();
	if (pipe_seq->childs_size == 1)
		return_code = execute_single_command_pipe_seq(pipe_seq, io);
	else
		return_code = execute_multi_command_pipe_seq(pipe_seq, io);
	child_reaper_unlock();
	sh()->return_code = return_code;
	return (return_code);
}

