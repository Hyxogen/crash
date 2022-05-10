/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   expansion.c                                      -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:59:46 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:59:46 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"

#include "memory.h"
#include "minishell.h"
#include <stdlib.h>

void
	expansion_destroy(t_expand *exp)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < exp->count)
	{
		j = 0;
		while (exp->parts[i].str[j] != NULL)
		{
			free(exp->parts[i].str[j]);
			j += 1;
		}
		free(exp->parts[i].str);
		i += 1;
	}
	free(exp->parts);
}

void
	expansion_init(t_expand *exp)
{
	exp->parts = NULL;
	exp->count = 0;
}

void
	expansion_add_part(t_expand *exp, char **str, int quote)
{
	exp->parts = sh_safe_reallog(exp->parts,
			sizeof(*exp->parts) * exp->count,
			sizeof(*exp->parts) * (exp->count + 1));
	exp->parts[exp->count].str = str;
	exp->parts[exp->count].quote = quote;
	exp->parts[exp->count].normal = 0;
	exp->parts[exp->count].array = 0;
	exp->parts[exp->count].tilde_expanded = 0;
	exp->count += 1;
}

void
	expansion_copy_parts(t_expand *dst, t_expand *src)
{
	size_t	i;

	i = 0;
	while (i < src->count)
	{
		dst->parts = sh_safe_reallog(dst->parts,
				sizeof(*dst->parts) * dst->count,
				sizeof(*dst->parts) * (dst->count + 1));
		dst->parts[dst->count].str = sh_strlst_dup(src->parts[i].str);
		dst->parts[dst->count].quote = src->parts[i].quote;
		dst->parts[dst->count].normal = src->parts[i].normal;
		dst->parts[dst->count].array = src->parts[i].array;
		dst->count += 1;
		i += 1;
	}
}
