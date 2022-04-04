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
	*sh_strlst_join(char **strs, char delim)
{
	char	*result;
	char	**cpy;
	size_t	tot_len;
	size_t	len;

	tot_len = 0;
	cpy = strs;
	while (*cpy)
	{
		tot_len += ft_strlen(*cpy) + 1;
		cpy += 1;
	}
	result = sh_safe_malloc(tot_len);
	cpy = strs;
	while (*cpy)
	{
		len = ft_strlen(*cpy);
		ft_memcpy(result, *cpy, len);
		result[len] = delim;
		cpy += 1;
		result += len + 1;
	}
	*result = '\0';
	return (result - (tot_len - 1));
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
