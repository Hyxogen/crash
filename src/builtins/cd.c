#include "minishell.h"
#include <unistd.h>
#include <string.h>
#include <errno.h>

/* TODO: CDPATH environment variable */
/* TODO: maybe PWD and OLDPWD environment variables */

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
			sh_err2("cd", strerror(errno));
	sh()->pwd = sh_getcwd();
	return (0);
}
