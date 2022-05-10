/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   wrap.c                                           -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:47 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:47 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int
	sh_dup2(int fildes, int fildes2)
{
	int	ret;

	ret = dup2(fildes, fildes2);
	sh_check(ret >= 0, "dup2");
	return (ret);
}

int
	sh_dup(int fildes)
{
	int	ret;

	ret = dup(fildes);
	sh_check(ret >= 0, "dup");
	return (ret);
}

int
	sh_pipe(int fildes[2])
{
	int	ret;

	ret = pipe(fildes);
	sh_check(ret >= 0, "pipe");
	return (ret);
}

pid_t
	sh_fork(void)
{
	pid_t	ret;

	ret = fork();
	sh_check(ret >= 0, "fork");
	if (ret == 0)
		sh()->is_subshell = 1;
	return (ret);
}

/* TODO: what execve errors to handle? */
int
	sh_execve(const char *path, char *const argv[], char *const envp[])
{
	int	ret;

	ret = execve(path, argv, envp);
	sh_check(ret >= 0, "execve");
	return (ret);
}
