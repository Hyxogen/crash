#include "minishell.h"
#include "commander.h"
#include "memory.h"
#include <stdlib.h>

char
	**_array_add(char **array, char *value)
{
	size_t	i;

	i = 0;
	while (array[i] != NULL)
		i += 1;
	array = sh_safe_realloc(array,
			sizeof(*array) * (i + 1),
			sizeof(*array) * (i + 2));
	array[i] = value;
	array[i + 1] = NULL;
	return (array);
}

char
	**cm_word_list_to_array(const t_snode *word_list)
{
	char	**ret;
	char	**tmp;
	size_t	i;
	size_t	j;

	sh_assert(word_list->type == sx_wordlist);
	ret = sh_safe_malloc(sizeof(*ret));
	ret[0] = NULL;
	i = 0;
	while (i < word_list->childs_size)
	{
		tmp = cm_wildcard_expand(&word_list->childs[i]->token);
		j = 0;
		if (tmp == NULL)
		{
			sh_strlst_clear(ret);
			return (NULL);
		}
		while (tmp[j] != NULL)
			ret = _array_add(ret, tmp[j++]);
		free(tmp);
		i += 1;
	}
	return (ret);
}
