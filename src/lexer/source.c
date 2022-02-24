/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   source.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/22 11:44:41 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/24 15:51:00 by dmeijer       ########   odam.nl         */
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
		while (line && *line && *str && (*line == *str || *line == '\\'))
		{
			if (*line == *str)
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

void
	_src_super_nom(t_lexer *lex, const char *str)
{
	while (*str)
	{
		lex_nom(lex, *str);
		str++; 
	}
	lex_nom(lex, '\n');
}

void
	_src_nom(void *ele, void *context)
{
	char	*str;
	t_lexer *lex;

	str = (char*) ele;
	lex = context;
	if (!str)
		return ;
	_src_super_nom(lex, str);
}


int
	src_check_end(t_lexer *lex, const char *end, int flags)
{
	ssize_t	ret;
	size_t	line_len;
	char	*line_str;

	if (!lex || lex->src->lst || !lex->src->str)
		return (0);
	line_str = lex->src->str;
	line_len = lex->src->len;
	while ((flags & HERE_FLAG_TRIM) && *line_str == '\t')
	{
		line_str++;
		line_len--;
	}
	while (!line_str || (!(flags & HERE_FLAG_QUOTE) && line_len > 0 && line_str[line_len - 1] == '\\'))
	{
		ret = _src_add_next(lex->src, &line_str);
		if (ret < 0)
			return (-1);
		if (ret == 0)
			break ;
		line_len = (size_t) ret;
	}
	if (_src_cmp(lex->src, end))
	{
		_src_super_nom(lex, lex->src->str);
		free(lex->src->str);
		lex->src->cur = -1;
		lex->src->nex = -1;
		lex->src->str = NULL;
		lex->src->off = 0;
		ft_lstforeach(lex->src->lst, _src_nom, lex);
		ft_lstclear(&lex->src->lst, free);
		return (1);
	}
	return (0);
}
