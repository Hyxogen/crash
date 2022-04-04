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
#include <stdio.h>

static void
	expadd(char ***list, int *new, int tmp)
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

static int
	expand_part(t_minishell *sh, t_tpart *part, char ***list, size_t *i)
{
	if (part->id == lx_parameter)
	{
		*list = cm_expand_param(sh, part->data);
		*i += *list != NULL;
		return (*list != NULL);
	}
	*list = sh_safe_malloc(sizeof(**list) * 2);
	(*list)[0] = NULL;
	(*list)[1] = NULL;
	if (part->id == lx_normal && part->data == NULL)
		(*list)[0] = (ft_strdup(""));
	if (part->id == lx_normal && part->data != NULL)
		(*list)[0] = (ft_strdup(part->data));
	if (part->id == lx_command)
		(*list)[0] = (cm_expand_command(sh, part->data));
	if (part->id == lx_backtick)
		(*list)[0] = (cm_expand_backtick(sh, part->data));
	// TODO: arithmetic expansion
	*i += (*list)[0] != NULL;
	return ((*list)[0] != NULL);
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
					expadd(list, new, 0);
				*new = 2;
			}
		}
		else
			expadd(list, new, str[i]);
		i += 1;
	}
}

// TODO: proper word splitting for arrays
static int
	expand_int(t_minishell *sh, t_token *token, t_token_ctx *ctx, int nosplit)
{
	size_t	i[3];
	int		new;
	char	*ifs;

	new = 2;
	i[0] = 0;
	ifs = sh_getenv(sh, "IFS", " \t\n");
	while (i[0] < token->count)
	{
		i[1] = 0;
		while (ctx->list[i[0]][i[1]] != NULL)
		{
			i[2] = 0;
			if (i[1] > 0)
				new = 2;
			if (token->parts[i[0]].quote)
				expadd(&ctx->fields, &new, 0);
			if (token->parts[i[0]].quote || token->parts[i[0]].id == lx_normal
				|| nosplit)
				while (ctx->list[i[0]][i[1]][i[2]] != '\0')
					expadd(&ctx->fields, &new, ctx->list[i[0]][i[1]][i[2]++]);
			else
				expand_split(&ctx->fields, &new, ctx->list[i[0]][i[1]], ifs);
			i[1] += 1;
		}
		i[0] += 1;
	}
	return (0);
}

char
	**cm_expand(t_minishell *sh, t_token *token, int nosplit)
{
	t_token_ctx	ctx;
	size_t		i[3];

	ctx.list = sh_safe_malloc(sizeof(*ctx.list) * token->count);
	ctx.fields = NULL;
	i[0] = 0;
	while (i[0] < token->count)
		if (!expand_part(sh, &token->parts[i[0]], &ctx.list[i[0]], &i[0]))
			break ;
	if (i[0] == token->count)
	{
		ctx.fields = sh_safe_malloc(sizeof(*ctx.fields));
		ctx.fields[0] = NULL;
		expand_int(sh, token, &ctx, nosplit);
	}
	i[1] = 0;
	while (i[1] < i[0])
	{
		i[2] = 0;
		while (ctx.list[i[1]][i[2]] != NULL)
			free(ctx.list[i[1]][i[2]++]);
		free(ctx.list[i[1]++]);
	}
	free(ctx.list);
	return (ctx.fields);
}
