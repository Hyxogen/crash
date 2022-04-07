#include "commander.h"
#include "minishell.h"
#include "memory.h"

void
	expansion_destroy(t_expansion *xps, size_t count)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < count)
	{
		j = 0;
		while (xps[i].fields[j] != NULL)
		{
			free(xps[i].fields[j]);
			j += 1;
		}
		free(xps[i].fields);
		i += 1;
	}
	free(xps);
}

int
	expand_part(t_minishell *sh, t_expand *out, t_tpart *part)
{
	if (part->id == lx_normal)
		return (expand_normal(sh, out, part->data));
	if (part->id == lx_command)
		return (expand_command(sh, out, part->data));
	if (part->id == lx_backtick)
		return (expand_backtick(sh, out, part->data));
	if (part->id == lx_parameter)
		return (expand_param(sh, out, part->data));
	if (part->id == lx_arithmetic)
		return (expand_arith(sh, out, part->data));
	sh_assert(0);
	return (-1);
}

int
	expand_collate(t_minishell *sh, t_expand *out, t_expand *xps, size_t count)
{
	char	*ifs;
	int		new;
	size_t	i;
	size_t	j;

	ifs = sh_getenv(sh, "IFS", " \t\n");
	new = 2;
	i = 0;
	while (i < token->count)
	{
		j = 0;
		while (xps[i].fields[j] != NULL)
		{
			if (j > 0)
				new = 2;
			j += 1;
		}
		i += 1;
	}
}

t_expand
	*cm_expand_list(t_minishell *sh, t_token *token)
{
	t_expand	*xps;
	size_t		i;

	xps = sh_safe_malloc(sizeof(*xps) * token->count);
	while (i < token->count)
	{
		if (expand_part(sh, &xps[i], &token->parts[i]) < 0)
			break ;
		i += 1;
	}
	if (i == token->count)
		return (xps);
	expansion_destroy(xps, i);
	free(xps);
	return (NULL);
}

int
	cm_expand(t_minishell *sh, t_expand *out, t_token *token)
{
	t_expand	*xps;
	int			result;

	xps = cm_expand_list(sh, token);
	if (xps == NULL)
		return (-1);
	result = expand_collate(sh, out, xps, token->count);
	expansion_destroy(xps, token->count);
	return (result);
}