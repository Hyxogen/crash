#include "commander.h"
#include "minishell.h"

#include <stdlib.h>

t_envvar
	*_do_assignment(const char *ass, int is_tmp)
{
	char		*tmp;
	size_t		j;
	t_envvar	*result;

	tmp = ft_strdup(ass);
	j = 0;
	while (tmp[j] != '=')
		j += 1;
	tmp[j] = '\0';
	result = sh_setenv(tmp, tmp + j + 1, is_tmp);
	free(tmp);
	return (result);
}

int
	_do_assignments(const t_snode *ass_list, int is_tmp)
{
	size_t	i;
	char	*tmp;

	i = 0;
	while (i < ass_list->childs_size)
	{
		tmp = cm_expand_str(&ass_list->childs[i]->token, NULL, ' ', 2);
		if (tmp == NULL)
		{
			sh_env_clean();
			return (-1);
		}
		_do_assignment(tmp, is_tmp);
		free(tmp);
		i += 1;
	}
	return (0);
}
