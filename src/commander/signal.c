/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   signal.c                                         -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:22:30 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:22:30 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"
#include "minishell.h"

#include <errno.h>
#include <signal.h>

static void
	_cm_reap_childs(void)
{
	int	rc;

	while (1)
	{
		rc = waitpid(0, NULL, WUNTRACED | WNOHANG);
		if (rc < 0)
		{
			sh_assert(errno == ECHILD);
			break ;
		}
		else if (rc == 0)
			break ;
	}
}

void
	_cm_child_reaper(int sig)
{
	int	old_errno;

	(void) sig;
	old_errno = errno;
	_cm_reap_childs();
	errno = old_errno;
}

int
	cm_disable_reaper(void)
{
	sh()->child_reaper.sa_handler = NULL;
	sh_sigaction(SIGCHLD, &sh()->child_reaper, NULL);
	return (0);
}

int
	cm_enable_reaper(void)
{
	sigemptyset(&sh()->child_reaper.sa_mask);
	sh()->child_reaper.sa_handler = _cm_child_reaper;
	sh()->child_reaper.sa_flags = SA_RESTART;
	sh_sigaction(SIGCHLD, &sh()->child_reaper, NULL);
	_cm_reap_childs();
	return (0);
}
