/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   source.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/22 11:44:41 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/01 15:37:56 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdlib.h>
#include "parser.h"

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

	if (src->str == NULL || src->off > src->len)
	{
		free(src->str);
		src->off = 0;
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
		return ('\n');
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
	_src_cmp(const t_source *src, const char *str, int flags)
{
	const t_list	*next;
	const char		*line;

	line = src->str;
	next = src->lst;
	while (line && (flags & flag_trim) && *line == '\t')
		line++;
	while (line && *line && *str && (*line == *str || *line == '\\'))
	{
		if (*line == *str)
			str++;
		line++;
		if (!*line)
		{
			if (next)
			{
				line = next->content;
				next = next->next;
			}
			else
				line = NULL;
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
	t_lexer	*lex;

	str = (char *) ele;
	lex = context;
	if (!str)
		return ;
	_src_super_nom(lex, str);
}

void
	_src_reset(t_source *src)
{
	free(src->str);
	src->cur = -1;
	src->nex = -1;
	src->str = NULL;
	src->off = 0;
}

int
	src_check_end(t_lexer *lex, const char *end, int flags)
{
	ssize_t	ret;

	if (!lex || lex->src->lst || !lex->src->str)
		return (0);
	while (!lex->src->str || (!(flags & flag_quote)
			&& lex->src->len > 0 && lex->src->str[lex->src->len - 1] == '\\'))
	{
		ret = _src_add_next(lex->src, &lex->src->str);
		if (ret < 0)
			return (-1);
		if (ret == 0)
			break ;
		lex->src->len = (size_t) ret;
	}
	if (_src_cmp(lex->src, end, flags))
	{
		_src_super_nom(lex, lex->src->str);
		_src_reset(lex->src);
		ft_lstforeach(lex->src->lst, _src_nom, lex);
		ft_lstclear(&lex->src->lst, free);
		return (1);
	}
	return (0);
}
