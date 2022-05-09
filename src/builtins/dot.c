#include "commander.h"
#include "minishell.h"

#include <stdio.h>
#include <fcntl.h>

/* NOTE implement if we ever wish to do so. */
/* We decided we couldn't really be bothered */
static int
	_is_binary(const char *filen)
{
	(void) filen;
	return (0);
}

/* Norm array contents */
/* 0: ret */
/* 1: fd */
/* 2: old_interactive */
static int
	_dot_run(int argc, char **argv)
{
	t_input	in;
	char	**old_args;
	char	*old_argv_0;
	int		norm[3];

	if (argc != 2)
	{
		old_argv_0 = argv[0];
		old_args = sh()->args;
		sh()->args = argv;
		sh()->args[0] = old_args[0];
	}
	norm[1] = sh_open(argv[1], O_RDONLY, 0);
	input_new(&in, in_file, (void *)(unsigned long long) norm[1]);
	norm[2] = sh()->interactive;
	sh()->interactive = 0;
	norm[0] = sh_cm_run(&in);
	sh()->interactive = norm[2];
	input_destroy(&in);
	sh_close(norm[1]);
	if (argc != 2)
		argv[0] = old_argv_0;
	if (argc != 2)
		sh()->args = old_args;
	return (norm[0]);
}

int
	sh_dot(int argc, char **argv)
{
	if (argc <= 1)
	{
		sh_err2(".", "filename argument required");
		return (-1);
	}
	if (_is_binary(argv[1]))
	{
		sh_err3(".", argv[1], "cannot execute binary file");
		return (-1);
	}
	return (_dot_run(argc, argv));
}
