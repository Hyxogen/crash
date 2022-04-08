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

static void
	_pattern_debug_print_pattern(t_pattern_node *head)
{
	size_t	index;
	size_t	size;

	size = sizeof(head->chars) / sizeof(head->chars[0]);
	while (head)
	{
		printf("[");
		index = 0;
		while (index < size)
		{
			if (head->chars[index])
				printf("%c", (char) index);
			index++;
		}
		head = head->next;
		printf("]");
	}
	printf("\n");
}

int
	pattern_match(const char *str, char *pattern, int *info)
{
	t_pattern_node	*head;
	int				match;

	head = _pattern_generate(pattern, info);
	sh_assert(head != NULL);
	_pattern_debug_print_pattern(head);
	match = _pattern_match(str, head);
	_pattern_destroy(head);
	return (match);
}
