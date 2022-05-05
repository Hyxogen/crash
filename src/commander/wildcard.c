#include "minishell.h"
#include "commander.h"
#include "memory.h"
#include "libft.h"

#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>

void
	cm_wildcard_add(char *filename, char ***out)
{
	size_t	i;

	i = 0;
	while ((*out)[i] != NULL)
		i += 1;
	*out = sh_safe_reallog(*out,
		sizeof(**out) * (i + 1),
		sizeof(**out) * (i + 2));
	(*out)[i] = ft_strdup(filename);
	(*out)[i + 1] = NULL;
}

char
	*cm_wildcard_join(char *prefix, char *name, char *suffix)
{
	char	*result;
	size_t	size;

	size = ft_strlen(prefix) + ft_strlen(name) + ft_strlen(suffix) + 1;
	result = sh_safe_malloc(size);
	result[0] = '\0';
	ft_strlcat(result, prefix, size);
	ft_strlcat(result, name, size);
	ft_strlcat(result, suffix, size);
	return (result);
}

void
	cm_wildcard_int(char *prefix, char *rest, int *info, char ***out)
{
	size_t			i;
	t_pattern_node	*pattern;
	DIR				*dir;
	struct dirent	*ent;
	char			*tmp;
	int				slash;
	int				has_match;

	if (*rest == '\0')
	{
		cm_wildcard_add(prefix, out);
		return ;
	}
	i = 0;
	while (rest[i] != '/' && rest[i] != '\0')
		i += 1;
	slash = rest[i] == '/';
	rest[i] = '\0';
	pattern = pattern_compile(rest, info);
	if (prefix[0] == '\0')
		dir = opendir(".");
	else
		dir = opendir(prefix);
	has_match = 0;
	if (dir != NULL)
	{
		while (1)
		{
			ent = readdir(dir);
			if (ent == NULL)
				break ;
			if (pattern_match(ent->d_name, pattern, 1))
			{
				tmp = cm_wildcard_join(prefix, ent->d_name, &"/"[!slash]);
				cm_wildcard_int(tmp, rest + i + slash, info + i + 1, out);
				free(tmp);
				has_match = 1;
			}
		}
		pattern_destroy(pattern);
		closedir(dir);
	}
	if (!has_match)
	{
		tmp = cm_wildcard_join(prefix, rest, &"/"[!slash]);
		cm_wildcard_int(tmp, rest + i + slash, info + i + 1, out);
		free(tmp);
	}
}

void
	cm_wildcard(const char *path, int *info, char ***out)
{
	int		root;
	char	*prefix;
	char	*tmp;

	root = path[0] == '/';
	prefix = ft_strndup(path, root);
	tmp = ft_strdup(path);
	cm_wildcard_int(prefix, tmp + root, info + root, out);
	free(prefix);
	free(tmp);
}

char
	**cm_wildcard_expand(const t_token *token)
{
	char	**list;
	int		**info;
	size_t	i;
	char	**out;

	list = cm_expand(token, &info);
	if (list == NULL)
		return (NULL);
	out = sh_safe_malloc(sizeof(*out));
	out[0] = NULL;
	i = 0;
	while (list[i] != NULL)
	{
		cm_wildcard(list[i], info[i], &out);
		i += 1;
	}
	i = 0;
	while (list[i] != NULL)
	{
		free(info[i]);
		i += 1;
	}
	free(info);
	sh_strlst_clear(list);
	return (out);
}
