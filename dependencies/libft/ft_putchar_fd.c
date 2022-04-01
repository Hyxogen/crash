/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putchar_fd.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/18 13:30:42 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/01/18 13:30:42 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

int
	ft_putchar_fd(char ch, int fd)
{
	return (write(fd, &ch, 1));
}
