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

/* TODO fix ": $(echo hallo)" */

int
	main(int argc, char **argv, char **envp)
{
	t_minishell	sh;
	t_input		in;
	char		*tmp;
	int			fd;
	t_builtin	builtins[4];

	(void) argc;
	builtins[0].key = "echo";
	builtins[0].fn = sh_echo;
	builtins[1].key = "exit";
	builtins[1].fn = sh_exit;
	builtins[2].key = ".";
	builtins[2].fn = sh_dot;
	builtins[3].key = ":";
	builtins[3].fn = sh_colon;
	tmp = getcwd(NULL, 0);
	sh.self = sh_join_path(tmp, argv[0]);
	sh.builtins = builtins;
	sh.builtins_size = 4;
	sh.args = argv + argc;
	sh.interactive = 1;
	free(tmp);
	sh_env_init(&sh, envp);
	cm_enable_reaper(&sh);
	if (argc >= 2)
	{
		fd = open(argv[1], O_RDONLY);
		sh.args = argv + 1;
		sh.interactive = 0;
		input_new(&in, in_file, (void*)(unsigned long long) fd);
	}
	else
		input_new(&in, in_readline, NULL);
	return (sh_cm_run(&sh, &in), EXIT_SUCCESS);
}
