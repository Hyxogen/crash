#include "minishell.h"
#include "memory.h"
#include "libft.h"

#include <unistd.h>

void
	cm_wildcard_add(char *filename, char ***out)
{
	size_t	i;

	i = 0;
	while ((*out)[i] != NULL)
		i += 1;
	*out = sh_safe_realloc(*out,
		sizeof(**out) * (i + 1),
		sizeof(**out) * (i + 2));
	(*out)[i] = ft_strdup(filename);
	(*out)[i + 1] = NULL;
}

void
	cm_wildcard_int(char *prefix, char **components, char ***out)
{
	if (*components == NULL)
	{
		sh_strlst_add(prefix, out);
		return ;
	}
}

char
	**cm_wildcard(char *path)
{
	char	**out;
	char	**components;
	int		root;

	out = sh_safe_malloc(sizeof(*out));
	out[0] = NULL;
	root = path[0] == '/';
	path += root;
	components = ft_split(path, '/');
	cm_wildcard_int()
}