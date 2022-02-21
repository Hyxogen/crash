/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_substr.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/18 13:30:49 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/21 15:19:28 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../../include/memory.h"
#include <stdlib.h>

char
	*ft_substr(const char *str, unsigned int idx, size_t max)
{
	size_t	len;
	char	*res;

	if (FT_SAFE >= 1 && str == NULL)
		return (NULL);
	len = ft_strlen(str);
	if (idx >= len)
		idx = len;
	if (idx + max >= len)
		max = len - idx;
	res = sh_safe_malloc(max + 1);
	if (res == NULL)
		return (NULL);
	ft_memcpy(res, str + idx, max);
	res[max] = '\0';
	return (res);
}
