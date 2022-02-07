/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memmove.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/18 13:30:41 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/01/18 13:30:41 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void
	*ft_memmove(void *dst, const void *src, size_t size)
{
	if (src >= dst)
		return (ft_memcpy(dst, src, size));
	while (0 < size)
	{
		size -= 1;
		((unsigned char *) dst)[size] = ((unsigned char *) src)[size];
	}
	return (dst);
}
