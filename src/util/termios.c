#include "minishell.h"

#include <sys/ioctl.h>
#include <errno.h>

void
	sh_get_term_attr(t_mega_termios *attr)
{
	tcgetattr(sh()->io[SH_STDIN_INDEX], &attr->term_attr[SH_STDIN_INDEX]);
	tcgetattr(sh()->io[SH_STDOUT_INDEX], &attr->term_attr[SH_STDOUT_INDEX]);
	tcgetattr(sh()->io[SH_STDERR_INDEX], &attr->term_attr[SH_STDERR_INDEX]);
}

void
	sh_set_term_attr(t_mega_termios *attr)
{
	tcsetattr(sh()->io[SH_STDIN_INDEX],
		TCSANOW, &attr->term_attr[SH_STDIN_INDEX]);
	tcsetattr(sh()->io[SH_STDOUT_INDEX],
		TCSANOW, &attr->term_attr[SH_STDOUT_INDEX]);
	tcsetattr(sh()->io[SH_STDERR_INDEX],
		TCSANOW, &attr->term_attr[SH_STDERR_INDEX]);
}

int
	sh_has_term(int fd)
{
	struct winsize	winsize;
	int				rc;

	rc = ioctl(fd, TIOCGWINSZ, &winsize);
	if (rc == 0)
		return (1);
	if (errno == ENOTTY)
		return (0);
	return (-1);
}
