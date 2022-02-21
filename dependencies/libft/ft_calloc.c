/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_calloc.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/18 13:30:35 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/21 15:19:09 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../../include/memory.h"
#include <stdlib.h>

void
	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = sh_safe_malloc(count * size);
	if (ptr == NULL)
		return (NULL);
	ft_bzero(ptr, count * size);
	return (ptr);
}
