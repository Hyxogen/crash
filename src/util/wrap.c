/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   wrap.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/25 14:22:11 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/25 15:32:44 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>

int
	sh_dup2(int fildes, int fildes2)
{
	int	ret;

	ret = dup2(fildes, fildes2);
	sh_check(ret >= 0, "dup2");
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

int
	sh_waitpid(pid_t pid, int *stat_loc, int options)
{
	int	ret;

	ret = waitpid(pid, stat_loc, options);
	sh_check(ret >= 0, "waitpid");
	return (ret);
}

int
	sh_close(int fildes)
{
	int	ret;

	ret = close(fildes);
	sh_check(ret >= 0, "close");
	return (ret);
}

/* TODO: Handle errors that should crash CraSH */
int
	sh_open(const char *path, int oflag, mode_t mode)
{
	int	ret;

	if (oflag & O_CREAT)
		ret = open(path, oflag, mode);
	else
		ret = open(path, oflag);
	if (ret <= 0)
		perror("CraSH");
	return (ret);
}
