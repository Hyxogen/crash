#include "commander.h"

#include <unistd.h>

// ODOT: option flags
int
	expand_special_minus(t_expand *exp, char *key)
{
	if (key[0] == '-')
	{
		expansion_add_part(exp, sh_strlst_new(ft_strdup("")), 0);
		return (1);
	}
	return (0);
}

int
	expand_special_dollar(t_expand *exp, char *key)
{
	if (key[0] == '$')
	{
		expansion_add_part(exp, sh_strlst_new(ft_itoa(getpid())), 0);
		return (1);
	}
	return (0);
}

int
	expand_special_bang(t_expand *exp, char *key)
{
	if (key[0] == '!')
	{
		if (sh()->last_bg_proc >= 0)
			expansion_add_part(exp,
				sh_strlst_new(ft_itoa(sh()->last_bg_proc)), 0);
		else
			expansion_add_part(exp, sh_strlst_new(ft_strdup("")), 0);
		return (1);
	}
	return (0);
}

int
	expand_special_digit(t_expand *exp, char *key, long i)
{
	long	idx;

	if (ft_isdigit(key[0]))
	{
		idx = ft_atol(key);
		if (idx < 0 || idx >= i)
			expansion_add_part(exp, sh_strlst_new(ft_strdup("")), 0);
		else
			expansion_add_part(exp,
				sh_strlst_new(ft_strdup(sh()->args[idx])), 0);
		return (1);
	}
	return (0);
}
