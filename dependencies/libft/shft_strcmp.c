/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shft_strcmp.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/22 11:20:57 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/25 14:22:17 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int
	ft_strcmp(const char *str1, const char *str2)
{
	return (ft_memcmp(str1, str2, ft_strlen(str1) + 1));
}
