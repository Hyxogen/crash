/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   pipe_sequence.c                                  -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:22:25 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:22:25 by csteenvo            :      ..            */
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

char **_word_list_to_array(t_snode *word_list)
{
	char	**ret;
	size_t	index;

	sh_assert(word_list->type == sx_wordlist);
	index = word_list->childs_size;
	ret = sh_safe_malloc(sizeof(*ret) * (index + 1));
	ret[index] = 0;
	index -= 1;
	while (index + 1)
	{
		ret[index] = word_list->childs[index]->token.str;
		index--;
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

int
	commandeer_simple_cmd(t_minishell *sh, t_snode *cmd_node, int fd_in, int fd_out)
{
	pid_t	pid;
	char	**args;

	(void) cmd_node;
	pid = sh_fork();
	if (pid == 0)
	{
		args = _word_list_to_array(cmd_node->childs[0]);
		dup2(fd_in, STDIN_FILENO);
		dup2(fd_out, STDOUT_FILENO);
		_cm_setup_redirects(sh, cmd_node->childs[1]);
		if (fd_in != STDIN_FILENO)
			close(fd_in);
		if (fd_out != STDOUT_FILENO)
			close(fd_out);
		sh_execvp(sh, args);
		_sh_execvp_error_handler(args[0], errno);
	}
	return (0);
}

int
	_commandeer_pipe_sequence(t_minishell *sh, t_snode *seq_node, int fd_out)
{
	size_t	index;
	size_t	count;
	int		io[2];
	int		prev_out;

	index = 0;
	prev_out = STDIN_FILENO;
	count = seq_node->childs_size;
	while (index < count)
	{
		if (index < (count - 1))
		{
			sh_pipe(io);
			commandeer_simple_cmd(sh, seq_node->childs[index], prev_out, io[1]);
			sh_close(io[1]);
		}
		else
			commandeer_simple_cmd(sh, seq_node->childs[index], prev_out, fd_out);
		if (prev_out != STDIN_FILENO)
			sh_close(prev_out);
		prev_out = io[0];
		index++;
	}
	if (fd_out != STDOUT_FILENO)
		sh_close(fd_out);
	return (0);
}

int
	commandeer_pipe_sequence(t_minishell *sh, t_snode *seq_node, void *data)
{
	int		status;
	int		rc;

	(void) data;
	if (seq_node->childs_size == 1)
		rc = commandeer_simple_cmd(sh, seq_node->childs[0], STDIN_FILENO, 1);
	else
		rc = _commandeer_pipe_sequence(sh, seq_node, 1);
	while (waitpid(0, &status, WUNTRACED) > 0)
		continue ;
	sh_assert(errno = ECHILD);
	return (_get_exit_code(status));
}
