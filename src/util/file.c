#include "minishell.h"

#include <unistd.h>
#include <errno.h>


#include <stdio.h>

void
	sh_fdctl(int fd, int flag, int on)
{
	fprintf(stderr, "%d close on exec:%d\n", fd, on);
	sh_assert(fd >= 0 && fd <= OPEN_MAX);
	sh()->fd_flags[fd] |= flag * !!on;
}

void
	sh_fd_before_exec(void)
{
	size_t	index;
	int		*flags;

	index = 0;
	flags = sh()->fd_flags;
	fprintf(stderr, "sh_fd_before_exec\n");
	while (index < OPEN_MAX)
	{
		if (flags[index] & SH_FD_FIOCLEX)
		{
			fprintf(stderr, "Closing %d\n", flags[index]);
			close(index);
			sh_assert(!(errno <= 0 && errno != EBADF));
		}
		index++;
	}
}
