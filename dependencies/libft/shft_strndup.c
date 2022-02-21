/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strndup.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/21 15:46:49 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/21 15:47:46 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include "../../include/memory.h"

char
	*ft_strndup(const char *s1, size_t n)
{
	size_t		len;
	char		*dup;
	const char	*cpy;

	len = 0;
	cpy = s1;
	while (*cpy && n)
	{
		len++;
		cpy++;
		n--;
	}
	dup = sh_safe_malloc(len + 1);
	if (dup == NULL)
		return (NULL);
	ft_memcpy(dup, s1, len);
	dup[len] = '\0';
	return (dup);
}
