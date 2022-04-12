/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   source.c                                         -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:22:52 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:22:52 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"
#include "parser.h"
#include <stdlib.h>

ssize_t
	_src_next_line(t_source *src, char **out)
{
	t_list	*tmp;

	if (src->lst)
	{
		*out = src->lst->content;
		tmp = src->lst->next;
		ft_lstdelone(src->lst, sh_nop);
		src->lst = tmp;
		return (ft_strlen(*out));
	}
	return (input_get_line(src->in, out));
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
	src_check_end(t_lexer *lex, const char *end, int flags)
{
	ssize_t		ret;
	char		*str;

	if (!lex || lex->src->lst || !lex->src->str)
		return (0);
	str = lex->src->str;
	ret = lex->src->len;
	while (!str || (!(flags & flag_quote)
			&& ret > 0 && str[ret - 1] == '\\'))
	{
		ret = _src_add_next(lex->src, &str);
		if (ret <= 0)
			break ;
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
