/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   main.c                                           -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:22:30 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:22:30 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "lexer.h"
#include "input.h"
#include "commander.h"
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <libgen.h>

extern void rl_clear_history(void);

void
	run_start_scripts(void)
{
	t_input		in;
	int			fd;
	const char	*home;
	char		*rc;
	int			old_interactive;

	home = sh_getenv("HOME", NULL);
	if (home != NULL)
	{
		rc = ft_strjoin(home, "/.crashrc");
		fd = open(rc, O_RDONLY);
		if (fd >= 0)
		{
			input_new(&in, in_file, (void*)(unsigned long long) fd);
			old_interactive = sh()->interactive;
			sh()->interactive = 0;
			sh_cm_run(&in);
			sh()->interactive = old_interactive;
			input_destroy(&in);
			close(fd);
		}
		free(rc);
	}	
}

int
	main(int argc, char **argv, char **envp)
{
	t_input		in;
	int			fd;

	sh_init(argv, envp);
	enable_signal_child_reaper_handler();
	if (argc >= 2)
	{
		fd = open(argv[1], O_RDONLY);
		sh_fdctl(fd, SH_FD_FIOCLEX, 1);
		sh_strlst_clear(sh()->args);
		sh()->args = sh_strlst_dup(argv + 1);
		sh()->interactive = 0;
		input_new(&in, in_file, (void *)(unsigned long long) fd);
	}
	else if (sh_has_term(STDIN_FILENO) == 1)
	{
		disable_kill_signals();
		run_start_scripts();
		rl_clear_history();
		input_new(&in, in_readline, NULL);
	} else {
		sh()->interactive = 0;
		input_new(&in, in_file, (void *) (unsigned long long) STDIN_FILENO);
	}
	sh_cm_run(&in);
	input_destroy(&in);
	sh_destroy();
	return (EXIT_SUCCESS);
}
