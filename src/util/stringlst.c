#include "minishell.h"
#include "memory.h"

#include <stdlib.h>

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

void
	sh_stringlst_end(t_stringlst *lst, char ***string, t_strinfo ***info)
{
	size_t	i;

	if (string != NULL)
	{
		*string = sh_safe_malloc(sizeof(**string) * (lst->size + 1));
		i = 0;
		while (i < lst->size)
		{
			(*string)[i] = ft_strdup(lst->strings[i].str);
			i += 1;
		}
		(*string)[i] = NULL;
	}
	if (info != NULL)
	{
		*info = sh_safe_malloc(sizeof(**info) * (lst->size + 1));
		i = 0;
		while (i < lst->size)
		{
			(*info)[i] = sh_safe_malloc(sizeof(*(*info)[i]) * (lst->strings[i].size + 1));
			ft_memcpy((*info)[i], lst->strings[i].info, sizeof(*(*info)[i]) * (lst->strings[i].size + 1));
			i += 1;
		}
		(*info)[i] = NULL;
	}
	i = 0;
	while (i < lst->size)
	{
		free(lst->strings[i].str);
		free(lst->strings[i].info);
		i += 1;
	}
	free(lst->strings);
}
