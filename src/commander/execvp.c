/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   execvp.c                                         -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:21:59 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:22:01 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"
#include "memory.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

static void
	sh_free_list(char **list)
{
	size_t	i;

	i = 0;
	while (list[i] != NULL)
	{
		free(list[i]);
		i += 1;
	}
	free(list);
}

static int
	sh_exec( char *name, char **argv)
{
	int		ret;
	size_t	i;
	char	**argv2;
	char	**envp;

	envp = sh_env();
	ret = execve(name, argv, envp);
	if (ret == -1 && errno == ENOEXEC)
	{
		i = 0;
		while (argv[i] != NULL)
			i += 1;
		argv2 = sh_safe_malloc(sizeof(*argv2) * (i + 2));
		argv2[i + 1] = NULL;
		while (i > 0)
		{
			argv2[i] = ft_strdup(argv[i - 1]);
			i -= 1;
		}
		argv2[0] = ft_strdup(sh()->self);
		ret = execve(sh()->self, argv2, envp);
		sh_free_list(argv2);
	}
	sh_free_list(envp);
	return (ret);
}

int
	sh_execvp( char **argv)
{
	size_t	i;
	char	**path;
	char	*strings[2];
	int		ret;

	/* TODO: some of this needs to move before builtin check */
	strings[0] = sh_getenv("PATH", NULL);
	if (ft_strchr(argv[0], '/') != NULL || strings[0] == NULL)
		return (sh_exec(argv[0], argv));
	/* TODO: check if splitting by : is correct */
	path = ft_split(strings[0], ':');
	i = 0;
	while (path[i] != NULL)
	{
		strings[1] = sh_join2(path[i], '/', argv[0]);
		if (access(strings[1], X_OK) == 0)
		{
			ret = sh_exec(strings[1], argv);
			free(strings[1]);
			sh_free_list(path);
			return (ret);
		}
		free(strings[1]);
		i += 1;
	}
	sh_free_list(path);
	return (-1);
}
