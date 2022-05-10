#include "commander.h"

#include "memory.h"
#include <libft.h>
#include <stdlib.h>

#define SH_PATTERN_ESCAPED	0x1

static void
	_pattern_init_node(t_pattern_node *node)
{
	ft_memset(node->chars, 0, sizeof(node->chars) / sizeof(node->chars[0]));
	node->infinite = 0;
	node->invert = 0;
	node->wildcard = 0;
	node->next = NULL;
}

// static void
// 	_pattern_add_node(t_pattern_node **head, t_pattern_node *node)
// {
// 	t_pattern_node	*temp;

// 	temp = *head;
// 	if (!*head)
// 	{
// 		*head = node;
// 		return;
// 	}
// 	temp = *head;
// 	while (temp->next)
// 		temp = temp->next;
// 	temp->next = node;
// }

static void
	_pattern_enable_all(t_pattern_node *node)
{
	ft_memset(&node->chars[0],
		0xff,
		sizeof(node->chars) / sizeof(node->chars[0]));
}

static void
	_pattern_generate_body(char **pattern, int **info, t_pattern_node *current)
{
	if (*(*info) & SH_PATTERN_ESCAPED)
		current->chars[(t_byte) *(*pattern)] = 0x1;
	else if (*(*pattern) == '[')
	{
		*info += _pattern_process_brackets(pattern, current, 0, 0);
		return ;
	}
	else if (*(*pattern) == '*')
	{
		_pattern_enable_all(current);
		current->wildcard = 1;
		current->infinite = 1;
	}
	else if (*(*pattern) == '?')
	{
		_pattern_enable_all(current);
		current->wildcard = 1;
	}
	else
		current->chars[(t_byte) *(*pattern)] = 0x1;
	*pattern += 1;
	*info += 1;
}

t_pattern_node
	*pattern_compile(char *pattern, int *info)
{
	t_pattern_node	*head;
	t_pattern_node	*current;
	t_pattern_node	**tail;

	head = NULL;
	tail = &head;
	while (*pattern)
	{
		current = sh_safe_malloc(sizeof(*head));
		_pattern_init_node(current);
		*tail = current;
		tail = &current->next;
		_pattern_generate_body(&pattern, &info, current);
	}
	return (head);
}

void
	pattern_destroy(t_pattern_node *node)
{
	t_pattern_node	*next;

	while (node)
	{
		next = node->next;
		free(node);
		node = next;
	}
}
