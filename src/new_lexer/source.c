/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   source.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/22 11:44:41 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/22 11:54:48 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "new_lexer.h"

void
	src_advance(t_source *src)
{
	ssize_t	str_len;

	if (!src->str || src->off >= src->len)
	{
		str_len = input_get_line(src->in, &src->str);
		if (str_len <= 0)
		{
			src->str = NULL;
			src->cur = 0;
			src->nex = 0;
			return ;
		}
		src->len = (size_t) str_len;
		src->off = 0;
		src->nex = src->str[0];
		return (src_advance(src));
	}
	src->off += 1;
	src->cur = src->nex;
	if (src->off >= src->len)
		src->nex = 0;
	else
		src->nex = src->str[src->off];
}
