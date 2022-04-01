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

int
	ft_putendl_fd(const char *str, int fd)
{
	int	rc;
	int	nwrite;

	if (FT_SAFE >= 1 && str == NULL)
		return (-1);
	nwrite = 0;
	rc = ft_putstr_fd(str, fd);
	if (rc < 0)
		return (-1);
	nwrite = rc;
	rc = ft_putchar_fd('\n', fd);
	if (rc < 0)
		return (-1);
	return (nwrite + rc);
}
