#include "commander.h"
#include "minishell.h"

#include <stdio.h>
#include <fcntl.h>

/* NOTE implement if we ever wish to do so. We decided we couldn't really be bothered */
static int
	_is_binary(const char *filen)
{
	(void) filen;
	return (0);
}

int
	sh_dot(int argc, char **argv)
{
	t_input	in;
	char	**old_args;
	char	*old_argv_0;
	int		ret;
	int		fd;

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
	fd = sh_open(argv[1], O_RDONLY, 0);
	if (argc != 2)
	{
		old_argv_0 = argv[0];
		old_args = sh()->args;
		sh()->args = argv;
		sh()->args[0] = old_args[0];
	}
	// TODO: should it be interactive?
	input_new(&in, in_file, (void*)(unsigned long long) fd);
	ret = sh_cm_run(&in);
	if (argc != 2)
	{
		argv[0] = old_argv_0;
		sh()->args = old_args;
	}
	return (ret);
}
