#include "commander.h"
#include "minishell.h"
#include "memory.h"
#include <libft.h>
#include <ft_printf.h>
#include <unistd.h>


#include <stdio.h>

static int
	_is_match(char ch, t_pattern_node *const node)
{
	if (node->invert)
		return (!node->chars[(size_t) ch]);
	return (!!node->chars[(size_t) ch]);
}

/* TODO rename node->infinite to something more appropriate */
void
	pattern_debug_print_node(t_pattern_node *node)
{
	unsigned int	index;

	index = 1;
	ft_fprintf(STDERR_FILENO, "[");
	while (index < sizeof(node->chars)/sizeof(node->chars[0]))
	{
		if (node->chars[index])
			ft_fprintf(STDERR_FILENO, "%c", index);
		index++;
	}
	ft_fprintf(STDERR_FILENO, "]");
}

void
	pattern_debug_print_chain(t_pattern_node *head)
{
	while (head)
	{
		pattern_debug_print_node(head);
		head = head->next;
	}
	ft_fprintf(STDERR_FILENO, "\n");
}

static int
	_pattern_match(const char *str, t_pattern_node *node)
{
	if (*str && node) {
		if (_is_match(*str, node))
		{
			if (node->infinite && _pattern_match(str + 1, node))
				return (1);
			if (_pattern_match(str + 1, node->next))
				return (1);
		}
		if (node->infinite)
			return (_pattern_match(str, node->next));
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

/* Returns 1 on match */
int
	pattern_match(const char *str, t_pattern_node *pattern)
{
	int				match;

	match = _pattern_match(str, pattern);
	return (match);
}
