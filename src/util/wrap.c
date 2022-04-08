/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   wrap.c                                           -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:23:41 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:23:44 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>


#include <execinfo.h>

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
	
	// TODO: FILE DESCRIPTORS! FILE DESCRIPTORS EVERYWHERE! THEY ARE COMING TO GET US! THEY NEED TO BE CLOSED! BUT NOT TWICE PLEASE!
	ret = close(fildes); // THERE IN THE TREES MAN, IN THE TREEEES!!!!
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

int
	sh_sigaction(int sig, const struct sigaction *act, struct sigaction *oact)
{
	int	ret;

	ret = sigaction(sig, act, oact);
	sh_check(ret >= 0, "sigaction");
	return (ret);
}

ssize_t
	sh_write(int fildes, const void *buf, size_t nbyte)
{
	int	ret;

	ret = write(fildes, buf, nbyte);
	sh_check(ret >= 0, "write");
	return (ret);
}
