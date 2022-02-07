/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putnbr_fd.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/18 13:30:43 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/01/18 13:30:43 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

void
	ft_putnbr_fd(int num, int fd)
{
	char	ch;

	if (num < 0)
		write(fd, "-", 1);
	ch = '0' + (num % 10) * (1 - (num < 0) * 2);
	if (num >= 10)
		ft_putnbr_fd(num / 10, fd);
	if (num <= -10)
		ft_putnbr_fd(-(num / 10), fd);
	write(fd, &ch, 1);
}
