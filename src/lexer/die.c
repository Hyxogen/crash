/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   die.c                                            -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:22:41 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:22:41 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parser.h"
#include <stdlib.h>

void
	tpart_destroy(t_tpart *part)
{
	if (part->id == lx_normal || part->id == lx_backtick)
	{
		free(part->data);
	}
	else if (part->id == lx_command)
	{
		node_destroy(part->data);
	}
	else if (part->id == lx_parameter || part->id == lx_arithmetic)
	{
		token_destroy(part->data);
		free(part->data);
	}
}

void
	token_destroy(t_token *tok)
{
	size_t	i;

	i = 0;
	free(tok->str);
	while (i < tok->count)
	{
		tpart_destroy(&tok->parts[i]);
		i += 1;
	}
	free(tok->parts);
}

void
	token_move(t_token *dst, t_token *src)
{
	*dst = *src;
	src->id = tk_invalid;
	src->parts = NULL;
	src->count = 0;
	src->str = NULL;
	src->len = 0;
}
