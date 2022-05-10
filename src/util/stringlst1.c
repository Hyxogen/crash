/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   stringlst1.c                                     -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:43 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:43 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "memory.h"

#include <stdlib.h>

static void
	sh_stringlist_strnotnull(t_stringlst *lst,
		char ***string, t_strinfo ***info)
{
	size_t	i;

	(void) info;
	*string = sh_safe_malloc(sizeof(**string) * (lst->size + 1));
	i = 0;
	while (i < lst->size)
	{
		(*string)[i] = ft_strdup(lst->strings[i].str);
		i += 1;
	}
	(*string)[i] = NULL;
}

static void
	sh_stringlist_infnotnull(t_stringlst *lst,
		char ***string, t_strinfo ***info)
{
	size_t	i;

	(void) string;
	*info = sh_safe_malloc(sizeof(**info) * (lst->size + 1));
	i = 0;
	while (i < lst->size)
	{
		(*info)[i]
			= sh_safe_malloc(sizeof(*(*info)[i]) * (lst->strings[i].size + 1));
		ft_memcpy((*info)[i],
			lst->strings[i].info,
			sizeof(*(*info)[i]) * (lst->strings[i].size + 1));
		i += 1;
	}
	(*info)[i] = NULL;
}

void
	sh_stringlst_end(t_stringlst *lst, char ***string, t_strinfo ***info)
{
	size_t	i;

	if (string != NULL)
		sh_stringlist_strnotnull(lst, string, info);
	if (info != NULL)
		sh_stringlist_infnotnull(lst, string, info);
	i = 0;
	while (i < lst->size)
	{
		free(lst->strings[i].str);
		free(lst->strings[i].info);
		i += 1;
	}
	free(lst->strings);
}
