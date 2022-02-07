/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putendl_fd.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/18 13:30:43 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/01/18 13:30:43 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void
	ft_putendl_fd(char *str, int fd)
{
	if (FT_SAFE >= 1 && str == NULL)
		return ;
	ft_putstr_fd(str, fd);
	ft_putchar_fd('\n', fd);
}
