#include "commander.h"

#include "memory.h"
#include <libft.h>


#include <stdio.h>
/**
 * Processes something called a collating symbols, which is (as far as I
 * understand) something to be handled as one character even though it
 * uses multiple characters. This is defined in the system's locale.
 *
 * CraSH uses Posix locale so only one character collating symbols like
 * [.a.] are supported
 *
 * A collating class always looks like this [.x.] where x is some
 * collating symbol
 *
 * This function must be called with the pattern starting with: [.
 */
int
	_pattern_process_collating_class(char **pattern, t_pattern_node *node, char *ch)
{
	if (*(*pattern + 1)
		&& *(*pattern + 2)
		&& *(*pattern + 3) == '.'
		&& *(*pattern + 4) == ']')
	{
		*ch = *(*pattern + 2);
		node->chars[(size_t) *(*pattern + 2)] = 0x1;
		*pattern += 5;
		return (5);
	}
	return (0);
}

/**
 * Processes equivalence classes which allow you to write [=a=] to mean
 * the characters [aàâ] etc.
 *
 * This function must be called with the pattern starting with: [=
 */
int
	_pattern_process_equivalence_class(char **pattern, t_pattern_node *node)
{
	if (*(*pattern + 2)
		&& *(*(pattern) + 3) == '='
		&& *(*(pattern) + 4) == ']')
	{
		node->chars[(size_t) *(*pattern + 2)] = 0x1;
		*pattern += 5;
		return (5);
	}
	return (0);
}

/* TODO rename */
static void
	_enable_chars(t_pattern_node *node, int (*isfunc)(int))
{
	unsigned int		index;
	size_t				size;

	index = 0;
	size = sizeof(node->chars) / sizeof(node->chars[0]);
	while (index < size)
	{
		if (isfunc(index))
			node->chars[index] = 0x1;
		index++;
	}
}

/**
 * Processes character classes, which is basically a feature to write human
 * readable patterns like: [[:digit:]][[:digit:]][[:digit:]][[:digit:]] to
 * mean a number with 4 digits
 *
 * This function must be called with the pattern starting with: [:
 */
int
	_pattern_process_char_class(char **pattern, t_pattern_node *node)
{
	if (!ft_strncmp(*pattern + 2, "alnum:]", 7))
		return (_enable_chars(node, ft_isalnum), *pattern += 9, 7);
	if (!ft_strncmp(*pattern + 2, "alpha:]", 7))
		return (_enable_chars(node, ft_isalpha), *pattern += 9, 7);
	if (!ft_strncmp(*pattern + 2, "blank:]", 7))
		return (_enable_chars(node, ft_isblank), *pattern += 9, 7);
	if (!ft_strncmp(*pattern + 2, "cntrl:]", 7))
		return (_enable_chars(node, ft_iscntrl), *pattern += 9, 7);
	if (!ft_strncmp(*pattern + 2, "digit:]", 7))
		return (_enable_chars(node, ft_isdigit), *pattern += 9, 7);
	if (!ft_strncmp(*pattern + 2, "graph:]", 7))
		return (_enable_chars(node, ft_isgraph), *pattern += 9, 7);
	if (!ft_strncmp(*pattern + 2, "lower:]", 7))
		return (_enable_chars(node, ft_islower), *pattern += 9, 7);
	if (!ft_strncmp(*pattern + 2, "print:]", 7))
		return (_enable_chars(node, ft_isprint), *pattern += 9, 7);
	if (!ft_strncmp(*pattern + 2, "punct:]", 7))
		return (_enable_chars(node, ft_ispunct), *pattern += 9, 7);
	if (!ft_strncmp(*pattern + 2, "space:]", 7))
		return (_enable_chars(node, ft_isspace), *pattern += 9, 7);
	if (!ft_strncmp(*pattern + 2, "alpha:]", 7))
		return (_enable_chars(node, ft_isupper), *pattern += 9, 7);
	if (!ft_strncmp(*pattern + 2, "xdigit:]", 8))
		return (_enable_chars(node, ft_isxdigit), *pattern += 10, 8);
	return (0);
}
