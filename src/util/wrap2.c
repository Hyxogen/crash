#include "minishell.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ft_printf.h>
#include <errno.h>
#include <string.h>

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
	sh_fdctl(fildes, SH_FD_FIOCLEX, 0);
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
	if (sh()->is_subshell)
		sh_check(ret >= 0, path);
	else
		if (ret < 0)
			sh_err2(path, strerror(errno));
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
