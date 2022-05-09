#include "commander.h"

#include "memory.h"
#include <libft.h>

/**
 * This function must be called with pattern starting at the first character
 * within a bracket group. For example 'a' in '[abcd]'
 */
static int
	_pattern_process_brackets_start(char **pattern, t_pattern_node *node)
{
	int	moved;

	moved = 1;
	*pattern += 1;
	if (*(*pattern) == '^')
	{
		node->invert = 1;
		*pattern += 1;
		moved += 1;
	}
	if (*(*pattern) == '-')
	{
		node->chars[(t_byte) *(*pattern)] = 0x1;
		*pattern += 1;
		moved += 1;
	}
	return (moved);
}

/* if (*(*pattern + 1) == '.') */
/* 	return (_pattern_process_collating_class(pattern, node)); */
static int
	_pattern_process_class(char **pattern, t_pattern_node *node)
{
	if (*(*pattern) == '[')
	{
		if (*(*pattern + 1) == '=')
			return (_pattern_process_equivalence_class(pattern, node));
		else if (*(*pattern + 1) == ':')
			return (_pattern_process_char_class(pattern, node));
	}
	return (0);
}

/**
 * Originally, this function was only meant to handle range, but 
 * because collating symbols can mix with range they were put togther
 *
 * Moved and ch only exist because of norminette, it doesn't matter
 * what the value is
 */
/* ODOT: check a pattern like [0-9 (yes no closing bracket) */
/* ODOT: Error when something goes wrong (return 0) */
static int
	_pattern_process_collating_range(char **ptn, t_pattern_node *n,
		int moved, char ch)
{
	moved = _pattern_process_collating_class(ptn, n, &ch);
	if (moved)
	{
		if (*(*ptn) == '-' && *(*ptn + 1) && *(*ptn) != ']')
		{
			if (ch > *(*ptn + 1))
				return (0);
			ft_memset(&n->chars[(t_byte) ch], 0x1, *(*ptn + 1) - ch);
			moved += 2;
			*ptn += 2;
		}
		return (moved);
	}
	if (*(*ptn) && *(*ptn + 1) == '-' && *(*ptn + 2))
	{
		if (*(*ptn + 2) != ']')
		{
			if (*(*ptn) > *(*ptn + 2))
				return (0);
			ft_memset(&n->chars[(t_byte) *(*ptn)], 0x1, *(*ptn + 2) - *(*ptn));
			*ptn += 3;
			return (moved + 3);
		}
	}
	return (0);
}

/* ODOT: make all pattern functions return size_t instead of int */
/**
 * Moved and local_moved only exist because of norminette, it doesn't matter
 * what the value is
 */
int
	_pattern_process_brackets(char **pattern, t_pattern_node *node,
		int moved, int local_moved)
{
	moved = _pattern_process_brackets_start(pattern, node);
	while (*(*pattern))
	{
		local_moved = _pattern_process_class(pattern, node);
		if (local_moved)
		{
			moved += local_moved;
			continue ;
		}
		if (*(*pattern) == ']')
		{
			*pattern += 1;
			return (moved + 1);
		}
		local_moved = _pattern_process_collating_range(pattern, node, 0, '\0');
		if (local_moved)
		{
			moved += local_moved;
			continue ;
		}
		node->chars[(t_byte) *(*pattern)] = 0x1;
		*pattern += 1;
		moved += 1;
	}
	return (0);
}
