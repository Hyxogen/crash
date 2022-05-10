#include "commander.h"

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

/* ODOT: maybe `int *quote` should be a char instead */
/* to save space for massive environment variables */
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
	expand_split(t_stringlst *lst, int *new, char *str, int quot)
{
	size_t	i;
	char	*ifs;

	ifs = sh_getenv("IFS", " \t\n");
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

/* I'm sorry for the variable names but norminette */
/* didn't like the readable version of this function */
void
	expand_collate(t_expand *e, t_stringlst *l)
{
	int		n;
	size_t	i;
	size_t	j;

	n = 2;
	i = 0;
	while (i < e->count)
	{
		j = 0;
		while (e->parts[i].str[j] != NULL)
		{
			if (j > 0)
				n = 2;
			if (e->parts[i].quote)
				expand_add(l, &n, '\0', e->parts[i].quote);
			if (e->parts[i].quote || e->parts[i].normal)
				expand_add_str(l, &n, e->parts[i].str[j], e->parts[i].quote);
			else
				expand_split(l, &n, e->parts[i].str[j], e->parts[i].quote);
			j += 1;
		}
		i += 1;
	}
}
