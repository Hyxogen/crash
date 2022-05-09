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
#include <stdio.h>
#include <readline/readline.h>
#include <libft.h>
#include <unistd.h>

extern int	rl_done;
extern void	rl_replace_line(const char *text, int clear_undo);

void
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

static void
	sigint_handler(int signal)
{
	(void) signal;
	rl_replace_line("", 1);
	sh()->restart = 1;
	if (sh()->loop_depth)
	{
		sh()->breaking = sh()->loop_depth;
		sh()->continuing = 1;
	}
	rl_done = 1;
	if (sh()->exec_count && sh()->interactive && !sh()->is_subshell)
		ft_putchar_fd('\n', STDOUT_FILENO);
}

void
	setup_default_signal_handlers(void)
{
	struct sigaction action;

	sigemptyset(&action.sa_mask);
	action.sa_handler = SIG_DFL;
	sh_sigaction(SIGQUIT, &action, NULL);
	sh_sigaction(SIGINT, &action, NULL);
}

void
	disable_kill_signals(void)
{
	struct sigaction action;

	sigemptyset(&action.sa_mask);
	action.sa_handler = SIG_IGN;
	action.sa_flags = SA_NODEFER | SA_RESTART;
	sh_sigaction(SIGQUIT, &action, NULL);
	action.sa_handler = sigint_handler;
	action.sa_flags = SA_RESTART;
	sh_sigaction(SIGINT, &action, NULL);
}
