#include "commander.h"
#include "memory.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>

static char
	*sh_join(char *lhs, char *rhs)
{
	size_t	size;
	char	*name;

	size = ft_strlen(lhs) + ft_strlen(rhs) + 2;
	name = sh_safe_malloc(size);
	name[0] = '\0';
	ft_strlcat(name, lhs, size);
	ft_strlcat(name, "/", size);
	ft_strlcat(name, rhs, size);
	return (name);
}

int
	sh_execvp(t_minishell *sh, char **argv)
{
	char	**path;
	char	*name;
	int		ret;
	size_t	i;

	if (ft_strchr(argv[0], '/') != NULL)
		return (execve(argv[0], argv, sh->environ));
	/* TODO: check if splitting by : is correct */
	/* TODO: sh_safe_malloc is not used */
	path = ft_split(sh->path, ':');
	i = 0;
	while (path[i] != NULL)
	{
		name = sh_join(path[i], argv[0]);
		ret = execve(name, argv, sh->environ);
		/* TODO: might not always want to continue search */
		free(name);
		i += 1;
	}
	i = 0;
	while (path[i] != NULL)
	{
		free(path[i]);
		i += 1;
	}
	free(path);
	return (ret);
}
