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
		node->chars[(unsigned char) *(*pattern)] = 0x1;
		*pattern += 1;
		moved += 1;
	}
	return (moved);
}

static int
	_pattern_process_class(char **pattern, t_pattern_node *node)
{
	if (*(*pattern) == '[')
	{
		// if (*(*pattern + 1) == '.')
		// 	return (_pattern_process_collating_class(pattern, node));
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
/* TODO check a pattern like [0-9 (yes no closing bracket) */
static int
	_pattern_process_collating_range(char **pattern, t_pattern_node *node, int moved, char ch)
{
	moved = _pattern_process_collating_class(pattern, node, &ch);
	if (moved)
	{
		if (*(*pattern) == '-' && *(*pattern + 1) && *(*pattern) != ']')
		{
			if (ch > *(*pattern + 1)) /* TODO error or something */
				return (0);
			ft_memset(&node->chars[(unsigned char) ch], 0x1, *(*pattern + 1) - ch);
			moved += 2;
			*pattern += 2;
		}
		return (moved);
	}
	if (*(*pattern) && *(*pattern + 1) == '-' && *(*pattern + 2))
	{
		if (*(*pattern + 2) != ']')
		{
			if (*(*pattern) > *(*pattern + 2)) /* TODO error or something */
				return (0);
			ft_memset(&node->chars[(unsigned char) *(*pattern)], 0x1, *(*pattern + 2) - *(*pattern));
			*pattern += 3;
			return (moved + 3);
		}
	}
	return (0);
}

/* TODO make all pattern functions return size_t instead of int */
/**
 * Moved and local_moved only exist because of norminette, it doesn't matter
 * what the value is
 */
int
	_pattern_process_brackets(char **pattern, t_pattern_node *node, int moved, int local_moved)
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
		node->chars[(unsigned char) *(*pattern)] = 0x1;
		*pattern += 1;
		moved += 1;
	}
	return (0);
}
