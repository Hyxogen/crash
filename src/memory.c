/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   memory.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/08 09:17:46 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/08 09:20:07 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

void
	*sh_safe_malloc(size_t size) {
	void	*ptr;

	ptr = malloc(size);
	sh_assert(ptr);
	return (ptr);
}

void
	*sh_reallloc(void *ptr, size_t old_size, size_t new_size) {
	unsigned char	*ret;

	ret = sh_safe_malloc(new_size);
	if (!ret)
		return (NULL);
	if (ptr)
	{
		ft_memcpy(ret, ptr, old_size);
		free(ptr);
	}
	return (ret);
}
