#include "commander.h"
#include "minishell.h"
#include "memory.h"
#include <stdlib.h>

void
	expand_tilde(t_epart *part, int first, int last, int ass)
{
	char		*str;
	t_stringlst	lst;

	if (!part->normal || part->quote || part->tilde_expanded)
		return ;
	part->tilde_expanded = 1;
	sh_stringlst_begin(&lst);
	sh_stringlst_add_string(&lst);
	str = *part->str;
	while (*str != '\0')
	{
		if (*str == '~' && first)
			str = expand_tilde1(str, &lst, last, ass);
		while ((*str != ':' || !ass) && *str != '\0')
		{
			sh_stringlst_add_char(&lst, *str, 0);
			str += 1;
		}
		if (*str == ':' && ass)
			sh_stringlst_add_char(&lst, *str, 0);
		str += (*str == ':' && ass);
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
			return (expansion_destroy(exp), -1);
		i += 1;
	}
	if (mode)
	{
		i = 0;
		while (i < exp->count)
		{
			expand_tilde(&exp->parts[i],
				i == 0, i == exp->count - 1, mode == 2);
			i += 1;
		}
	}
	return (0);
}

char
	*cm_expand_str_end(int **quote, t_stringlst *lst)
{
	char		**result;
	int			**info;
	char		*result0;

	sh_stringlst_end(lst, &result, &info);
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
	*cm_expand_str(const t_token *token, int **quote, int ch, int mode)
{
	t_expand	exp;
	t_stringlst	lst;
	size_t		i;

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
	return (cm_expand_str_end(quote, &lst));
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
