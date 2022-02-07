/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strchr.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/18 13:30:45 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/01/18 13:30:45 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char
	*ft_strchr(const char *str, int ch)
{
	while (1)
	{
		if (*str == (char) ch)
			return ((char *) str);
		if (!*str)
			return (NULL);
		str += 1;
	}
}
