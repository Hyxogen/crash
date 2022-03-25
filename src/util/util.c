#include "minishell.h"
#include "memory.h"
#include "libft.h"

char
	*sh_join2(char *lhs, char delim, char *rhs)
{
	size_t	size;
	char	*str;
	char	buf[2];

	size = ft_strlen(lhs) + ft_strlen(rhs) + 2;
	str = sh_safe_malloc(size);
	str[0] = '\0';
	buf[0] = delim;
	buf[1] = '\0';
	ft_strlcat(str, lhs, size);
	ft_strlcat(str, buf, size);
	ft_strlcat(str, rhs, size);
	return (str);
}

/* TODO: what to do if delim not found */
/* TODO: safe malloc not always used, also check for other functions */
void
	sh_split2(char *str, char delim, char **lhs, char **rhs)
{
	size_t	i;

	i = 0;
	while (str[i] != delim)
		i += 1;
	*lhs = sh_safe_malloc(i + 1);
	ft_memcpy(*lhs, str, i);
	(*lhs)[i] = '\0';
	*rhs = ft_strdup(&str[i + 1]);
}

char
	*sh_join_path(char *lhs, char *rhs)
{
	if (rhs[0] == '/')
		return (ft_strdup(rhs));
	return (sh_join2(lhs, '/', rhs));
}
