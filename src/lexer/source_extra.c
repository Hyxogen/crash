/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   source_extra.c                                   -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:22:53 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:22:53 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parser.h"

#include <stdlib.h>

int
	_src_cmp(const t_source *src, const char *str, int flags)
{
	const t_list	*next;
	const char		*line;

	line = src->str;
	next = src->lst;
	while (line && (flags & flag_trim) && *line == '\t')
		line++;
	while (line && *line && *str
		&& (*line == *str || (*(line + 1) == '\0' && *line == '\\')))
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
	return (!*str && (!line || !*line));
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
