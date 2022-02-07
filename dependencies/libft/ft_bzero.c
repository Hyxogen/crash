/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_bzero.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/18 13:30:34 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/01/18 13:30:34 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void
	ft_bzero(void *ptr, size_t size)
{
	ft_memset(ptr, 0, size);
}
