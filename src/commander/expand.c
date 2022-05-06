#include "commander.h"
#include "minishell.h"
#include "memory.h"
#include <stdlib.h>

int
	expand_part(t_expand *exp, const t_tpart *part, int mode)
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
		tmp = expand_param(exp, part->data, mode);
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

// ODOT: maybe `int *quote` should be a char instead to save space for massive environment variables

void
	expand_add(t_stringlst *lst, int *new, int tmp, int quot)
{
	if (*new)
	{
		sh_stringlst_add_string(lst);
		*new = 0;
	}
	if (tmp != '\0')
		sh_stringlst_add_char(lst, tmp, quot);
}

void
	expand_add_str(t_stringlst *lst, int *new, char *str, int quot)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		expand_add(lst, new, str[i], quot);
		i += 1;
	}
}

void
	expand_split(t_stringlst *lst, int *new, char *str, char *ifs, int quot)
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
					expand_add(lst, new, '\0', quot);
				*new = 2;
			}
		}
		else
			expand_add(lst, new, str[i], quot);
		i += 1;
	}
}

void
	expand_collate(t_expand *exp, t_stringlst *lst)
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
				expand_add(lst, &new, '\0', exp->parts[i].quote);
			if (exp->parts[i].quote || exp->parts[i].normal)
				expand_add_str(lst, &new, exp->parts[i].str[j], exp->parts[i].quote);
			else
				expand_split(lst, &new, exp->parts[i].str[j], ifs, exp->parts[i].quote);
			j += 1;
		}
		i += 1;
	}
}

void
	expand_str_add_part(t_stringlst *lst, t_epart *part, int delim)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (part->str[i] != NULL)
	{
		if (i != 0)
			sh_stringlst_add_char(lst, delim, part->quote);
		j = 0;
		while (part->str[i][j] != '\0')
		{
			sh_stringlst_add_char(lst, part->str[i][j], part->quote);
			j += 1;
		}
		i += 1;
	}
}

void
	expand_tilde(t_epart *part, int first, int last, int ass)
{
	char		*str;
	char		*tmp;
	t_stringlst	lst;
	const char	*home;

	if (!part->normal || part->quote || part->tilde_expanded)
		return ;
	part->tilde_expanded = 1;
	sh_stringlst_begin(&lst);
	sh_stringlst_add_string(&lst);
	str = *part->str;
	while (*str != '\0')
	{
		if (*str == '~' && first)
		{
			tmp = str;
			while (*tmp != '\0' && *tmp != '/' && (*tmp != ':' || !ass))
				tmp += 1;
			if ((*tmp == '\0' && !last) || tmp != str + 1)
			{
				while (str < tmp)
				{
					sh_stringlst_add_char(&lst, *str, 0);
					str += 1;
				}
			}
			else
			{
				home = sh_getenv("HOME", "~");
				while (*home != '\0')
				{
					sh_stringlst_add_char(&lst, *home, 0);
					home += 1;
				}
				str = tmp;
			}
		}
		while ((*str != ':' || !ass) && *str != '\0')
		{
			sh_stringlst_add_char(&lst, *str, 0);
			str += 1;
		}
		if (*str == ':' && ass)
		{
			sh_stringlst_add_char(&lst, *str, 0);
			str += 1;
		}
		first = 1;
	}
	sh_strlst_clear(part->str);
	sh_stringlst_end(&lst, &part->str, NULL);
}

int
	cm_expand_list(t_expand *exp, const t_token *token, int mode)
{
	size_t	i;
	int		tmp;

	expansion_init(exp);
	i = 0;
	while (i < token->count)
	{
		tmp = mode;
		if (token->parts[i].quote)
			tmp = 0;
		if (expand_part(exp, &token->parts[i], tmp) < 0)
		{
			expansion_destroy(exp);
			return (-1);
		}
		i += 1;
	}
	if (mode)
	{
		i = 0;
		while (i < exp->count)
		{
			expand_tilde(&exp->parts[i], i == 0, i == exp->count - 1, mode == 2);
			i += 1;
		}
	}
	return (0);
}

char
	*cm_expand_str(const t_token *token, int **quote, int ch, int mode)
{
	t_expand	exp;
	t_stringlst	lst;
	size_t		i;
	char		**result;
	int			**info;
	char		*result0;

	if (cm_expand_list(&exp, token, mode) < 0)
		return (NULL);
	sh_stringlst_begin(&lst);
	sh_stringlst_add_string(&lst);
	i = 0;
	while (i < exp.count)
	{
		expand_str_add_part(&lst, &exp.parts[i], ch);
		i += 1;
	}
	expansion_destroy(&exp);
	sh_stringlst_end(&lst, &result, &info);
	result0 = result[0];
	if (quote != NULL)
		*quote = info[0];
	else
		free(info[0]);
	free(result);
	free(info);
	return (result0);
}

char
	**cm_expand(const t_token *token, int ***quotes, int mode)
{
	t_expand	exp;
	char		**fields;
	t_stringlst	lst;

	if (cm_expand_list(&exp, token, mode) < 0)
		return (NULL);
	sh_stringlst_begin(&lst);
	expand_collate(&exp, &lst);
	sh_stringlst_end(&lst, &fields, quotes);
	expansion_destroy(&exp);
	return (fields);
}
