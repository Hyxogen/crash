/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   memory.c                                         -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:41 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:41 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <libft.h>
#include <stdlib.h>

void
	*sh_safe_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	SH_ASSERT(ptr != NULL);
	return (ptr);
}

void
	*sh_safe_realloc(void *ptr, size_t old_size, size_t new_size)
{
	size_t			size;
	unsigned char	*ret;

	ret = sh_safe_malloc(new_size);
	if (!ret)
		return (NULL);
	size = old_size;
	if (new_size < old_size)
		size = new_size;
	if (ptr)
	{
		ft_memcpy(ret, ptr, size);
		free(ptr);
	}
	return (ret);
}

static size_t
	sh_safe_reallog_p2(size_t size)
{
	if (size == 0)
		return (1);
	size -= 1;
	size |= size >> 1;
	size |= size >> 2;
	size |= size >> 4;
	size |= size >> 8;
	size |= size >> 16;
	size |= size >> 32;
	size += 1;
	return (size);
}

void
	*sh_safe_reallog(void *ptr, size_t old_size, size_t new_size)
{
	old_size = sh_safe_reallog_p2(old_size);
	new_size = sh_safe_reallog_p2(new_size);
	if (old_size != new_size)
		return (sh_safe_realloc(ptr, old_size, new_size));
	return (ptr);
}
