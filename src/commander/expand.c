/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/25 16:22:19 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/03/28 10:20:48 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "memory.h"
#include "commander.h"
#include "libft.h"
#include <stdlib.h>

static void
	expand_add(char ***list, int *new, int tmp)
{
	size_t	i;
	size_t	len;

	i = 0;
	while ((*list)[i] != NULL)
		i += 1;
	if (*new || tmp == -1)
	{
		*list = sh_safe_realloc(*list, sizeof(**list) * (i + 1), sizeof(**list) * (i + 2));
		(*list)[i] = sh_safe_malloc(2);
		(*list)[i][0] = '\0';
		i += 1;
		(*list)[i] = NULL;
		*new = 0;
	}
	if (tmp != -1)
	{
		len = ft_strlen((*list)[i - 1]);
		(*list)[i - 1] = sh_safe_realloc((*list)[i - 1], len + 1, len + 2);
		(*list)[i - 1][len] = tmp;
		(*list)[i - 1][len + 1] = '\0';
	}
}

static char
	*expand_part(t_minishell *sh, t_tpart *part)
{
	if (part->id == lx_normal)
	{
		if (part->data == NULL)
			return (ft_strdup(""));
		return (ft_strdup(part->data));
	}
	if (part->id == lx_parameter)
		return (cm_expand_param(sh, part->data));
	sh_assert(0);
}

static void
	expand_split(char ***list, int *new, const char *str, const char *ifs)
{
	size_t	i;
	int		is_ifs;

	i = 0;
	while (str[i] != '\0')
	{
		is_ifs = ft_strchr(ifs, str[i]) != NULL;
		if (is_ifs && ft_strchr("\n\t\r\v\f ", str[i]) == NULL)
			expand_add(list, new, str[i]);
		if (is_ifs)
			*new = 1;
		else
			expand_add(list, new, str[i]);
		i += 1;
	}
}

static int
	expand_int(t_minishell *sh, t_token *token, char ***list, const char *ifs)
{
	int			new;
	size_t		i;
	size_t		j;
	char		*tmp;

	new = 1;
	i = 0;
	while (i < token->count)
	{
		tmp = expand_part(sh, &token->parts[i]);
		if (tmp == NULL)
			return (1);
		j = 0;
		if (token->parts[i].id == lx_normal)
			while (tmp[j] != '\0')
			{
				expand_add(list, &new, tmp[j]);
				j += 1;
			}
		else
			expand_split(list, &new, tmp, ifs);
		free(tmp);
		i += 1;
	}
	return (0);
}

/* TODO: properly expand token */
char
	**cm_expand(t_minishell *sh, t_token *token)
{
	t_envvar	*ifs_var;
	const char	*ifs;
	char		**list;
	size_t		i;

	// TODO: maybe IFS needs to be taken before simple command is executed
	ifs_var = sh_getenv(sh, "IFS");
	ifs = " \t\n";
	if (ifs_var != NULL)
		ifs = ifs_var->value;
	list = sh_safe_malloc(sizeof(*list) * 2);
	list[0] = NULL;
	if (expand_int(sh, token, &list, ifs))
	{
		i = 0;
		while (list[i] != NULL)
		{
			free(list[i]);
			i += 1;
		}
		free(list);
		return (NULL);
	}
	return (list);
}
