/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   source.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/22 11:44:41 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/24 13:10:36 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdlib.h>

ssize_t
	_src_next_line(t_source *src, char **out)
{
	t_list	*tmp;

	if (src->lst)
	{
		free(src->str);
		*out = src->lst->content;
		tmp = src->lst->next;
		ft_lstdelone(src->lst, sh_nop);
		src->lst = tmp;
		return (ft_strlen(*out));
	}
	return (input_get_line(src->in, out));
}

ssize_t
	_src_add_next(t_source *src, char **out)
{
	char	*str;
	ssize_t	len;

	len = input_get_line(src->in, &str);
	if (len <= 0)
		return (len);
	ft_lstadd_back(&src->lst, ft_lstnew(str));
	*out = str;
	return (len);
}

// void
// 	src_advance(t_source *src)
// {
// 	ssize_t	str_len;

// 	if (!src->str || src->off >= src->len || src->nex == -1)
// 	{
// 		str_len = _src_next_line(src, &src->str);
// 		if (str_len <= 0)
// 		{
// 			src->str = NULL;
// 			src->cur = -1;
// 			src->nex = -1;
// 			return ;
// 		}
// 		src->len = (size_t) str_len;
// 		src->off = 0;
// 		src->nex = src->str[0];
// 		src_advance(src);
// 		return ;
// 	}
// 	src->off += 1;
// 	src->cur = src->nex;
// 	if (src->off >= src->len)
// 		src->nex = '\n';
// 	else
// 		src->nex = src->str[src->off];
// }

int
	src_readchar(t_source *src)
{
	int		c;
	ssize_t	ret;

	if (src->str == NULL)
	{
		ret = _src_next_line(src, &src->str);
		if (ret < 0)
			return (-1);
		if (ret == 0)
			return (0);
		src->len = (size_t) ret;
	}
	if (src->str == NULL)
		return (-1);
	c = (unsigned char) src->str[src->off];
	src->off += 1;
	if (c == '\0')
	{
		free(src->str);
		src->str = NULL;
		src->off = 0;
		return ('\n');
	}
	return (c);
}

void
	src_advance(t_source *src)
{
	src->cur = src->nex;
	src->nex = -1;
	if (src->cur == -1)
		src->cur = src_readchar(src);
	if (src->cur != -1 && src->cur != '\n')
		src->nex = src_readchar(src);
}

int
	_src_cmp(const t_source *src, const char *str)
{
	const t_list	*current;
	const char 		*line;

	line = src->str;
	if (!line)
		return (0);
	while (*line && *str && *line == *str)
	{
		line++;
		str++;
	}
	current = src->lst;
	if (current)
	{
		line = current->content;
		while (line && *line && *str && *line == *str)
		{
			str++;
			line++;
			if (!*line)
			{
				if (current->next)
				{
					current = current->next;
					line = current->content;
				}
				else
					line = NULL;
			}
		}
	}
	return (!*str);
}

int
	src_check_end(t_source *src, const char *end, int flags)
{
	ssize_t	ret;
	size_t	line_len;
	char	*line_str;

	if (src->lst)
		return (0);
	line_str = src->str;
	line_len = src->len - src->off;
	while ((flags & HERE_FLAG_TRIM) && *line_str == '\t')
	{
		line_str++;
		line_len--;
	}
	while (!(flags & HERE_FLAG_QUOTE) && (!line_str || *(line_str + line_len - 1) == '\\'))
	{
		ret = _src_add_next(src, &line_str);
		if (ret < 0)
			return (-1);
		if (ret == 0)
			break ;
		line_len = (size_t) ret;
	}
	if (_src_cmp(src, end))
	{
		free(src->str);
		src->str = NULL;
		src->off = 0;
		ft_lstclear(&src->lst, free);
		return (1);
	}
	return (0);
}
