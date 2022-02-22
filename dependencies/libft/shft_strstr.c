/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shft_strstr.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/22 11:20:57 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/22 11:22:19 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libt.h"

char
	*ft_strstr(const char *str1, const char *str2)
{
	return (ft_strnstr(str1, str2, ft_strlen(str1)));	
}