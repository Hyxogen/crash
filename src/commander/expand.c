/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/25 14:03:15 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/25 14:24:55 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"
#include "libft.h"

/* TODO properly expand token */
const char
	*cm_expand(t_token *token)
{
	return ft_strdup(token->str);
}
