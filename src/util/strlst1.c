#include "minishell.h"
#include "memory.h"
#include "libft.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

void
	sh_strlst_clear(char **strs)
{
	char	**cpy;

	if (!strs)
		return ;
	cpy = strs;
	while (*cpy)
	{
		cpy += 1;
		free(*(cpy - 1));
	}
	free(strs);
}

char
	*sh_strlst_join(char **strs, int delim)
{
	char	*result;
	char	**cpy;
	size_t	tot_len;
	size_t	len;

	tot_len = 0;
	tot_len = sh_strlst_strsize(strs, delim);
	result = sh_safe_malloc(tot_len + 1);
	cpy = strs;
	while (*cpy)
	{
		if (delim != '\0' && cpy != strs)
		{
			result[0] = delim;
			result += 1;
		}
		len = ft_strlen(*cpy);
		ft_memcpy(result, *cpy, len);
		cpy += 1;
		result += len;
	}
	*result = '\0';
	return (result - tot_len);
}

char
	**sh_strlst_dup(char **strs)
{
	char	**lst;
	size_t	i;

	i = 0;
	while (strs[i] != NULL)
		i += 1;
	lst = sh_safe_malloc(sizeof(*lst) * (i + 1));
	i = 0;
	while (strs[i] != NULL)
	{
		lst[i] = ft_strdup(strs[i]);
		i += 1;
	}
	lst[i] = NULL;
	return (lst);
}

char
	**sh_strlst_new(char *str)
{
	char	**lst;

	lst = sh_safe_malloc(sizeof(*lst) * 2);
	lst[0] = str;
	lst[1] = NULL;
	return (lst);
}

char
	**sh_strlst_empty(void)
{
	char	**lst;

	lst = sh_safe_malloc(sizeof(*lst));
	lst[0] = NULL;
	return (lst);
}
