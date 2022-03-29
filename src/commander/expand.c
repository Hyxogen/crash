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
	if (*new)
	{
		*list = sh_safe_realloc(*list, sizeof(**list) * (i + 1), sizeof(**list) * (i + 2));
		(*list)[i] = sh_safe_malloc(1);
		(*list)[i][0] = '\0';
		i += 1;
		(*list)[i] = NULL;
		*new = 0;
	}
	if (tmp != '\0')
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
	if (part->id == lx_command)
		return (cm_expand_command(sh, part->data));
	if (part->id == lx_backtick)
		return (cm_expand_backtick(sh, part->data));
	sh_assert(0);
}

static void
	expand_split(char ***list, int *new, const char *str, const char *ifs)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (ft_strchr(ifs, str[i]) != NULL)
		{
			if (ft_strchr("\n\t\r\v\f ", str[i]) != NULL)
			{
				if (*new != 2)
					*new = 1;
			}
			else
			{
				if (*new == 2)
					expand_add(list, new, 0);
				*new = 2;
			}
		}
		else
			expand_add(list, new, str[i]);
		i += 1;
	}
}

static int
	expand_int(t_minishell *sh, t_token *token, char ***list, char **fields)
{
	size_t	i;
	size_t	j;
	int		new;
	char	*ifs;

	new = 2;
	i = 0;
	ifs = sh_getenv_default(sh, "IFS", " \t\n");
	while (i < token->count)
	{
		j = 0;
		if (token->parts[i].quote)
			expand_add(list, &new, 0);
		if (token->parts[i].quote || token->parts[i].id == lx_normal)
			while (fields[i][j] != '\0')
				expand_add(list, &new, fields[i][j++]);
		else
			expand_split(list, &new, fields[i], ifs);
		i += 1;
	}
	return (0);
}

// TODO: assignments should not be split
char
	**cm_expand(t_minishell *sh, t_token *token)
{
	char	**list;
	char	**fields;
	size_t	i;
	size_t	j;

	fields = sh_safe_malloc(sizeof(*fields) * token->count);
	list = NULL;
	i = 0;
	while (i < token->count)
	{
		fields[i] = expand_part(sh, &token->parts[i]);
		if (fields[i++] == NULL)
			break;
	}
	if (i == token->count)
	{
		list = sh_safe_malloc(sizeof(*list));
		list[0] = NULL;
		expand_int(sh, token, &list, fields);
	}
	j = 0;
	while (j < i)
		free(fields[j++]);
	free(fields);
	return (list);
}
