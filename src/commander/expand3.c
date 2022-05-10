#include "commander.h"

char
	*expand_tilde1(char *str, t_stringlst *lst, int last, int ass)
{
	char		*tmp;
	const char	*home;

	tmp = str;
	while (*tmp != '\0' && *tmp != '/' && (*tmp != ':' || !ass))
		tmp += 1;
	if ((*tmp == '\0' && !last) || tmp != str + 1)
	{
		while (str < tmp)
		{
			sh_stringlst_add_char(lst, *str, 0);
			str += 1;
		}
	}
	else
	{
		home = sh_getenv("HOME", "~");
		while (*home != '\0')
		{
			sh_stringlst_add_char(lst, *home, 0);
			home += 1;
		}
		str = tmp;
	}
	return (str);
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
