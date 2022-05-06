#include "commander.h"

int
	expand_special_asterisk(t_expand *exp, char *key)
{
	char	*str;

	if (key[0] == '*')
	{
		str = sh_getenv("IFS", NULL);
		if (str == NULL)
			str = sh_strlst_join(sh()->args + 1, ' ');
		else
			str = sh_strlst_join(sh()->args + 1, str[0]);
		expansion_add_part(exp, sh_strlst_new(str), 0);
		return (1);
	}
	return (0);
}

int
	expand_special_at(t_expand *exp, char *key)
{
	if (key[0] == '@')
	{
		expansion_add_part(exp, sh_strlst_dup(sh()->args + 1), 0);
		exp->parts[exp->count - 1].array = 1;
		return (1);
	}
	return (0);
}

int
	expand_special_sharp(t_expand *exp, char *key, long i)
{
	if (key[0] == '#')
	{
		expansion_add_part(exp, sh_strlst_new(ft_itoa(i - 1)), 0);
		return (1);
	}
	return (0);
}

int
	expand_special_qmark(t_expand *exp, char *key)
{
	if (key[0] == '?')
	{
		expansion_add_part(exp, sh_strlst_new(ft_itoa(sh()->return_code)), 0);
		return (1);
	}
	return (0);
}

int
	expand_special_minus(t_expand *exp, char *key)
{
	if (key[0] == '-')
	{
		// ODOT: option flags
		expansion_add_part(exp, sh_strlst_new(ft_strdup("")), 0);
		return (1);
	}
	return (0);
}
