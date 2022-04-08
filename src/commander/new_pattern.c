#include "commander.h"

#include "memory.h"
#include <libft.h>

static int
	_is_match(char ch, t_pattern_node *const node)
{
	return (!!node->chars[(size_t) ch]);
}

static int
	_pattern_match(const char *str, t_pattern_node *node)
{
	int	cmp;

	if (*str && node && _is_match(*str, node))
	{
		if (node->infinite)
		{
			cmp = _pattern_match(str + 1, node);
			if (cmp)
				return (1);
		}
		return (_pattern_match(str + 1, node->next));
	}
	if (*str)
		return (0);
	if (!node)
		return (1);
	while (node)
	{
		if (!node->infinite)
			return (0);
		node = node->next;
	}
	return (1);
}

int
	pattern_match(const char *str, char *pattern, int *info)
{
	t_pattern_node	*head;

	head = _pattern_generate(pattern, info);
	sh_assert(head != NULL);
	return (_pattern_match(str, head));
}
