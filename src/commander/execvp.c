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
	sh_exec(t_minishell *sh, char *name, char **argv)
{
	int		ret;
	size_t	i;
	char	**argv2;
	char	**envp;

	envp = sh_env(sh);
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
		argv2[0] = ft_strdup(sh->self);
		ret = execve(sh->self, argv2, envp);
		sh_free_list(argv2);
	}
	sh_free_list(envp);
	return (ret);
}

int
	sh_execvp(t_minishell *sh, char **argv)
{
	size_t	i;
	char	**path;
	char	*name;
	int		ret;

	if (ft_strchr(argv[0], '/') != NULL)
		return (sh_exec(sh, argv[0], argv));
	i = 0;
	while (i < sh->builtins_size)
	{
		if (ft_strcmp(argv[0], sh->builtins[i].key))
			return (sh->builtins[i].fn(sh, argv), 0);
		i += 1;
	}
	/* TODO: check if splitting by : is correct */
	/* TODO: sh_safe_malloc is not used */
	/* TODO: what if PATH is unset */
	path = ft_split(sh_getenv(sh, "PATH")->value, ':');
	i = 0;
	while (path[i] != NULL)
	{
		name = sh_join2(path[i], '/', argv[0]);
		if (access(name, X_OK) == 0)
		{
			ret = sh_exec(sh, name, argv);
			free(name);
			sh_free_list(path);
			return (ret);
		}
		free(name);
		i += 1;
	}
	sh_free_list(path);
	return (-1);
}
