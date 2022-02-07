/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putstr_fd.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/18 13:30:44 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/01/18 13:30:44 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

void
	ft_putstr_fd(char *str, int fd)
{
	if (FT_SAFE >= 1 && str == NULL)
		return ;
	write(fd, str, ft_strlen(str));
}
