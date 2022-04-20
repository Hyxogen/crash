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
	try_reap_all_childs(void)
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
	signal_try_reap_all_childs(int sig)
{
	int	old_errno;

	(void) sig;
	old_errno = errno;
	try_reap_all_childs();
	errno = old_errno;
}

void
	disable_signal_child_reaper_handler(void)
{
	sh()->child_reaper.sa_handler = NULL;
	sh_sigaction(SIGCHLD, &sh()->child_reaper, NULL);
}

void
	enable_signal_child_reaper_handler(void)
{
	sigemptyset(&sh()->child_reaper.sa_mask);
	sh()->child_reaper.sa_handler = signal_try_reap_all_childs;
	sh()->child_reaper.sa_flags = SA_RESTART;
	sh_sigaction(SIGCHLD, &sh()->child_reaper, NULL);
}

static int
	*get_lock_counter(void)
{
	static int	lock_counter = 0;

	return (&lock_counter);
}

int
	child_reaper_lock(void)
{
	int	*lock_counter;

	lock_counter = get_lock_counter();
	if (*lock_counter >= INT_MAX)
		return (-1);
	if (*lock_counter == 0)
		disable_signal_child_reaper_handler();
	*lock_counter += 1;
	return (0);
}

int
	child_reaper_unlock(void)
{
	int	*lock_counter;

	lock_counter = get_lock_counter();
	sh_assert(*lock_counter != 0);
	*lock_counter -= 1;
	if (*lock_counter == 0)
	{
		enable_signal_child_reaper_handler();
		try_reap_all_childs();
	}
	return (0);
}

