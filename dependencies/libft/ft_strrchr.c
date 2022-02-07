/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strrchr.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/18 13:30:49 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/01/18 13:30:49 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char
	*ft_strrchr(const char *str, int ch)
{
	char	*res;

	res = NULL;
	while (1)
	{
		if (*str == (char) ch)
			res = (char *) str;
		if (!*str)
			return (res);
		str += 1;
	}
}
