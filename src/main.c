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

int
	main(int argc, char **argv, char **envp)
{
	t_input		in;
	char		*tmp;
	int			fd;
	t_builtin	builtins[5];

	(void) argc;
	builtins[0].key = "echo";
	builtins[0].fn = sh_echo;
	builtins[1].key = "exit";
	builtins[1].fn = sh_exit;
	builtins[2].key = ".";
	builtins[2].fn = sh_dot;
	builtins[3].key = ":";
	builtins[3].fn = sh_colon;
	builtins[4].key = "set";
	builtins[4].fn = sh_set;
	tmp = getcwd(NULL, 0);
	sh()->self = sh_join_path(tmp, argv[0]);
	sh()->builtins = builtins;
	sh()->builtins_size = 5;
	sh()->args = argv;
	sh()->interactive = 1;
	sh()->io[SH_STDIN_INDEX] = STDIN_FILENO;
	sh()->io[SH_STDOUT_INDEX] = STDOUT_FILENO;
	sh()->io[SH_STDERR_INDEX] = STDERR_FILENO;
	tmp = ft_strdup(argv[0]);
	sh()->name = basename(tmp); // TODO: can't use basename
	free(tmp);
	sh_env_init(envp);
	cm_enable_reaper();
	if (argc >= 2)
	{
		fd = open(argv[1], O_RDONLY);
		sh()->args = argv + 1;
		sh()->interactive = 0;
		input_new(&in, in_file, (void*)(unsigned long long) fd);
	}
	else
		input_new(&in, in_readline, NULL);
	return (sh_cm_run(&in), EXIT_SUCCESS);
}
