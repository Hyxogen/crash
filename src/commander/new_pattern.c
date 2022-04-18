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

void
	pattern_debug_print_node(t_pattern_node *node)
{
	int	index;

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

/* Returns 1 on match */
int
	pattern_match(const char *str, char *pattern, int *info)
{
	t_pattern_node	*head;
	int				match;

	head = _pattern_generate(pattern, info);
	if (head == NULL)
		return (0);
	match = _pattern_match(str, head);
	_pattern_destroy(head);
	return (match);
}
