/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   memory.c                                         -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:23:35 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:23:35 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <libft.h>
#include <stdlib.h>

void
	*sh_safe_malloc(size_t size) {
	void	*ptr;

	ptr = malloc(size);
	sh_assert(ptr != NULL);
	return (ptr);
}

void
	*sh_safe_realloc(void *ptr, size_t old_size, size_t new_size) {
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
