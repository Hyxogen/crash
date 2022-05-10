/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   util.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/25 16:23:39 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/03/28 10:26:22 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "memory.h"
#include "libft.h"
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <execinfo.h>

char
	*sh_join2(const char *lhs, char delim, const char *rhs)
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

/* ODOT: what to do if delim not found */
void
	sh_split2(char *str, char delim, char **lhs, char **rhs)
{
	size_t	i;

	i = 0;
	while (str[i] != delim && str[i] != '\0')
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

int
	sh_exists(const char *filen)
{
	struct stat	info;

	if (!stat(filen, &info))
		return (1);
	SH_ASSERT(errno == ENOENT);
	return (0);
}

// TODO: debug function, remove!!!
#ifdef SH_BACKTRACE

void
	sh_backtrace(int count)
{
	void	**buffer;

	buffer = malloc(sizeof(*buffer) * (count + 1));
	count = backtrace(buffer, count + 1);
	backtrace_symbols_fd(buffer + 1, count - 1, STDERR_FILENO);
	free(buffer);
}

#else

void
	sh_backtrace(int count)
{
	(void) count;
}

#endif
