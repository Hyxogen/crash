/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memset.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/18 13:30:42 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/01/18 13:30:42 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void
	*ft_memset(void *ptr, int ch, size_t size)
{
	while (size)
	{
		size -= 1;
		((unsigned char *) ptr)[size] = ch;
	}
	return (ptr);
}
