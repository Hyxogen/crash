/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strmapi.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/18 13:30:47 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/21 15:19:25 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../../include/memory.h"
#include <stdlib.h>

char
	*ft_strmapi(const char *str, char (*func)(unsigned int, char))
{
	char			*res;
	unsigned int	i;

	if (FT_SAFE >= 1 && str == NULL)
		return (NULL);
	res = sh_safe_malloc(ft_strlen(str) + 1);
	if (res == NULL)
		return (NULL);
	i = 0;
	while (str[i])
	{
		res[i] = func(i, str[i]);
		i += 1;
	}
	res[i] = '\0';
	return (res);
}
