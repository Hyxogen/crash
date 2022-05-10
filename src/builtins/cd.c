#include "minishell.h"
#include <unistd.h>
#include <string.h>
#include <errno.h>

/* ODOT: CDPATH environment variable */
/* ODOT: maybe PWD and OLDPWD environment variables */

int
	sh_cd(int argc, char **argv)
{
	const char	*path;

	(void) argc;
	path = argv[1];
	if (path == NULL)
		path = sh_getenv("HOME", NULL);
	if (path != NULL)
		if (chdir(path) < 0)
			sh_err3("cd", argv[1], strerror(errno));
	sh()->pwd = sh_getcwd();
	return (0);
}
