#include "commander.h"
#include "minishell.h"
#include "memory.h"
#include <stdlib.h>

int
	expand_part(t_expand *exp, const t_tpart *part)
{
	size_t	i;
	int		tmp;

	i = exp->count;
	tmp = 0;
	if (part->id == lx_normal && part->data != NULL)
		expansion_add_part(exp, sh_strlst_new(ft_strdup(part->data)), 0);
	else if (part->id == lx_normal)
		expansion_add_part(exp, sh_strlst_new(ft_strdup("")), 0);
	else if (part->id == lx_command)
		tmp = expand_command(exp, part->data);
	else if (part->id == lx_backtick)
		tmp = expand_backtick(exp, part->data);
	else if (part->id == lx_parameter)
		tmp = expand_param(exp, part->data);
	else
		tmp = expand_arith(exp, part->data);
	while (tmp == 0 && i < exp->count)
	{
		exp->parts[i].normal = (part->id == lx_normal);
		exp->parts[i].quote = (exp->parts[i].quote || part->quote);
		i += 1;
	}
	return (tmp);
}

// TODO: NOTE: if this has to be refactored for norminette then we might as well do it properly by storing the length instead of using reallog with ft_strlen
// TODO: NOTE: also if maybe `int *quote` should be a char instead to save space for massive environment variables

void
	expand_add(char ***fields, int ***quotes, int *new, int tmp, int quot)
{
	size_t	i;
	size_t	len;

	i = 0;
	while ((*fields)[i] != NULL)
		i += 1;
	if (*new)
	{
		*fields = sh_safe_reallog(*fields,
			sizeof(**fields) * (i + 1),
			sizeof(**fields) * (i + 2));
		(*fields)[i] = sh_safe_malloc(1);
		(*fields)[i][0] = '\0';
		if (quotes != NULL)
		{
			*quotes = sh_safe_reallog(*quotes,
				sizeof(**quotes) * (i + 0),
				sizeof(**quotes) * (i + 1));
			(*quotes)[i] = sh_safe_malloc(sizeof(*(*quotes)[i]));
		}
		i += 1;
		(*fields)[i] = NULL;
		*new = 0;
	}
	if (tmp != '\0')
	{
		len = ft_strlen((*fields)[i - 1]);
		(*fields)[i - 1] = sh_safe_reallog((*fields)[i - 1], len + 1, len + 2);
		(*fields)[i - 1][len] = tmp;
		(*fields)[i - 1][len + 1] = '\0';
		if (quotes != NULL)
		{
			(*quotes)[i - 1] = sh_safe_reallog((*quotes)[i - 1],
				sizeof(***quotes) * (len + 0),
				sizeof(***quotes) * (len + 1));
			(*quotes)[i - 1][len] = quot;
		}
	}
}

void
	expand_add_str(char ***fields, int ***quotes, int *new, char *str, int quot)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		expand_add(fields, quotes, new, str[i], quot);
		i += 1;
	}
}

void
	expand_split(char ***fields, int ***quotes, int *new, char *str, char *ifs, int quot)
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
					expand_add(fields, quotes, new, '\0', quot);
				*new = 2;
			}
		}
		else
			expand_add(fields, quotes, new, str[i], quot);
		i += 1;
	}
}

void
	expand_collate(t_expand *exp, char ***fields, int ***quotes)
{
	char	*ifs;
	int		new;
	size_t	i;
	size_t	j;

	ifs = sh_getenv("IFS", " \t\n");
	new = 2;
	i = 0;
	while (i < exp->count)
	{
		j = 0;
		while (exp->parts[i].str[j] != NULL)
		{
			if (j > 0)
				new = 2;
			if (exp->parts[i].quote)
				expand_add(fields, quotes, &new, '\0', exp->parts[i].quote);
			if (exp->parts[i].quote || exp->parts[i].normal)
				expand_add_str(fields, quotes, &new, exp->parts[i].str[j], exp->parts[i].quote);
			else
				expand_split(fields, quotes, &new, exp->parts[i].str[j], ifs, exp->parts[i].quote);
			j += 1;
		}
		i += 1;
	}
}

void
	expand_str_add(char **str, int **quote, int ch, int quot)
{
	size_t	i;

	i = 0;
	while ((*str)[i] != '\0')
		i += 1;
	*str = sh_safe_reallog(*str, i + 1, i + 2);
	(*str)[i] = ch;
	(*str)[i + 1] = '\0';
	if (quote != NULL)
	{
		*quote = sh_safe_reallog(*quote,
			sizeof(**quote) * i,
			sizeof(**quote) * (i + 1));
		(*quote)[i] = quot;
	}
}

void
	expand_str_add_part(char **str, int **quote, t_epart *part, int delim)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (part->str[i] != NULL)
	{
		if (i != 0)
			expand_str_add(str, quote, delim, part->quote);
		j = 0;
		while (part->str[i][j] != '\0')
		{
			expand_str_add(str, quote, part->str[i][j], part->quote);
			j += 1;
		}
		i += 1;
	}
}

// TODO: check -1 when called
int
	cm_expand_list(t_expand *exp, const t_token *token)
{
	size_t	i;

	expansion_init(exp);
	i = 0;
	while (i < token->count)
	{
		if (expand_part(exp, &token->parts[i]) < 0)
		{
			expansion_destroy(exp);
			return (-1);
		}
		i += 1;
	}
	return (0);
}

// TODO: check null when called
char
	*cm_expand_str(const t_token *token, int **quote, int ch)
{
	t_expand	exp;
	char		*result;
	size_t		i;

	if (cm_expand_list(&exp, token) < 0)
		return (NULL);
	result = sh_safe_malloc(1);
	result[0] = '\0';
	i = 0;
	while (i < exp.count)
	{
		expand_str_add_part(&result, quote, &exp.parts[i], ch);
		i += 1;
	}
	expansion_destroy(&exp);
	return (result);
}

// TODO: check null when called
// TODO: tilde expansion and wildcards
char
	**cm_expand(const t_token *token, int ***quotes)
{
	t_expand	exp;
	char		**fields;

	if (cm_expand_list(&exp, token) < 0)
		return (NULL);
	fields = sh_strlst_empty();
	if (quotes != NULL)
	{
		*quotes = sh_safe_malloc(sizeof(**quotes));
		(*quotes)[0] = NULL;
	}
	expand_collate(&exp, &fields, quotes);
	expansion_destroy(&exp);
	return (fields);
}
