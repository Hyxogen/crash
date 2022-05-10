#include "minishell.h"
#include "memory.h"
#include "libft.h"

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
