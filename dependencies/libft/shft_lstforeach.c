/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shft_lstforeach.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/21 15:23:19 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/21 15:27:29 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void
	ft_lstforeach(const t_list *lst, void (*proc)(void *, void *), void *param)
{
	while (lst)
	{
		proc(lst->content, param);
		lst = lst->next;
	}
}
