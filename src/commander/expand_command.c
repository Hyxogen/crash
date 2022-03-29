#include "commander.h"
#include "memory.h"
#include <unistd.h>
#include <stdlib.h>

// TODO: verify
// TODO: trim newlines at end
static char
	*expand_command_fd(pid_t pid, int fd)
{
	char	*str;
	size_t	size;
	int		ret;

	str = NULL;
	size = 0;
	ret = 1;
	while (ret > 0)
	{
		str = sh_safe_realloc(str, size, size + 1024);
		ret = read(fd, str + size, 1024);
		if (ret >= 0)
			size += ret;
	}
	str[size] = '\0';
	sh_close(fd);
	sh_waitpid(pid, NULL, 0);
	return (str);
}

// TODO: verify
char
	*cm_expand_command(t_minishell *sh, t_snode *node)
{
	pid_t	pid;
	int		pipe_in[2];
	int		pipe_out[2];
	int		io[3];

	sh_pipe(pipe_in);
	sh_pipe(pipe_out);
	pid = sh_fork();
	if (pid == 0)
	{
		sh_close(pipe_in[1]);
		sh_close(pipe_out[0]);
		io[SH_STDIN_INDEX] = pipe_in[0];
		io[SH_STDOUT_INDEX] = pipe_out[1];
		io[SH_STDERR_INDEX] = 2;
		commandeer(sh, node, io);
		exit(EXIT_SUCCESS);
	}
	sh_close(pipe_in[0]);
	sh_close(pipe_in[1]);
	sh_close(pipe_out[1]);
	return expand_command_fd(pid, pipe_out[0]);
}

char
	*cm_expand_backtick(t_minishell *sh, char *str)
{
	(void) sh;
	(void) str;
	return ft_strdup("Segmentation success");
}
