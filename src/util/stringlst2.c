/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   stringlst2.c                                     -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:43 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:43 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "memory.h"

void
	sh_stringlst_begin(t_stringlst *lst)
{
	lst->strings = NULL;
	lst->size = 0;
}

void
	sh_stringlst_add_string(t_stringlst *lst)
{
	t_string	*last;

	lst->strings = sh_safe_reallog(lst->strings,
			sizeof(*lst->strings) * (lst->size + 0),
			sizeof(*lst->strings) * (lst->size + 1));
	last = &lst->strings[lst->size];
	last->str = sh_safe_malloc(1);
	last->str[0] = '\0';
	last->info = sh_safe_malloc(sizeof(*last->info));
	last->info[0] = 0;
	last->size = 0;
	lst->size += 1;
}

void
	sh_stringlst_add_char(t_stringlst *lst, int ch, t_strinfo info)
{
	t_string	*last;

	last = &lst->strings[lst->size - 1];
	last->str = sh_safe_reallog(last->str,
			last->size + 1,
			last->size + 2);
	last->info = sh_safe_reallog(last->info,
			sizeof(*last->info) * (last->size + 1),
			sizeof(*last->info) * (last->size + 2));
	last->str[last->size] = ch;
	last->info[last->size] = info;
	last->size += 1;
	last->str[last->size] = '\0';
	last->info[last->size] = 0;
}
