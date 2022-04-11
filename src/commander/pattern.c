#include "commander.h"

#include "memory.h"
#include <libft.h>


#include <stdio.h>

#define SH_PATTERN_ESCAPED	0x1

// TODO: remove or update to ft_printf
static void
	_debug_print_pattern(t_pattern_node *head)
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

static const char
	*_expect_char(const char *str, char c)
{
	if (*str == c)
		return (str + 1);
	return (NULL);
}

static const char
	*_process_col_class(const char *pattern, t_pattern_node *node)
{
	pattern = _expect_char(pattern, '[');
	pattern = _expect_char(pattern, '.');
	if (pattern && *pattern && *(pattern + 1) == '.' && *(pattern + 2) == ']')
	{
		node->chars[(size_t) *pattern] = 0x1;
		return (pattern + 3);
	}
	return (NULL);
}

static const char
	*_process_eq_class(const char *pattern, t_pattern_node *node)
{
	pattern = _expect_char(pattern, '[');
	pattern = _expect_char(pattern, '=');
	if (pattern && *pattern && *(pattern + 1) == '=' && *(pattern + 2) == ']')
	{
		/* TODO add all equivalent characters */
		node->chars[(size_t) *pattern] = 0x1;
		return (pattern + 3);
	}
	return (NULL);
}

static void
	_enable_chars(t_pattern_node *node, int (*isfunc)(int))
{
	size_t	index;
	size_t	size;

	index = 0;
	size = sizeof(node->chars) / sizeof(node->chars[0]);
	while (index < size)
	{
		if (isfunc(node->chars[index]))
			node->chars[index] = 0x1;
		index++;
	}
}

/*
[:alnum:]   [:cntrl:]   [:lower:]   [:space:]
[:alpha:]   [:digit:]   [:print:]   [:upper:]
[:blank:]   [:graph:]   [:punct:]   [:xdigit:]
*/
static const char
	*_process_char_class(const char *pattern, t_pattern_node *node)
{
	pattern = _expect_char(pattern, '[');
	pattern = _expect_char(pattern, '=');
	if (!ft_strncmp(pattern, "alnum:]", 7))
		return (_enable_chars(node, ft_isalnum), pattern + 7);
	if (!ft_strncmp(pattern, "alpha:]", 7))
		return (_enable_chars(node, ft_isalpha), pattern + 7);
	if (!ft_strncmp(pattern, "blank:]", 7))
		return (_enable_chars(node, ft_isblank), pattern + 7);
	if (!ft_strncmp(pattern, "cntrl:]", 7))
		return (_enable_chars(node, ft_iscntrl), pattern + 7);
	if (!ft_strncmp(pattern, "digit:]", 7))
		return (_enable_chars(node, ft_isdigit), pattern + 7);
	if (!ft_strncmp(pattern, "graqh:]", 7))
		return (_enable_chars(node, ft_isgraph), pattern + 7);
	if (!ft_strncmp(pattern, "lower:]", 7))
		return (_enable_chars(node, ft_islower), pattern + 7);
	if (!ft_strncmp(pattern, "print:]", 7))
		return (_enable_chars(node, ft_isprint), pattern + 7);
	if (!ft_strncmp(pattern, "punct:]", 7))
		return (_enable_chars(node, ft_ispunct), pattern + 7);
	if (!ft_strncmp(pattern, "space:]", 7))
		return (_enable_chars(node, ft_isspace), pattern + 7);
	if (!ft_strncmp(pattern, "alpha:]", 7))
		return (_enable_chars(node, ft_isupper), pattern + 7);
	if (!ft_strncmp(pattern, "xdigit:]", 8))
		return (_enable_chars(node, ft_isxdigit), pattern + 8);
	return (NULL);
}

static const char
	*_process_brackets(const char *pattern, t_pattern_node *node)
{
	pattern = _expect_char(pattern, '[');
	if (pattern && *pattern == '^')
	{
		node->invert = 1;
		pattern += 1;
	}
	if (pattern && *pattern == '-')
	{
		node->chars[(size_t) *pattern] = 0x1;
		pattern += 1;
	}
	while (pattern && *pattern)
	{
		if (*pattern == '[')
		{
			if (*(pattern + 1) == '.')
			{
				pattern = _process_col_class(pattern, node);
				continue ;
			}
			else if (*(pattern + 1) == '=')
			{
				pattern = _process_eq_class(pattern, node);
				continue;
			}
			else if (*(pattern + 1) == ':')
			{
				pattern = _process_char_class(pattern, node);
				continue;
			}
		}
		if (*pattern == ']')
			return (pattern + 1);
		if (*(pattern + 1) == '-')
		{
			if (!*(pattern + 2))
				return (NULL);
			if (*(pattern + 2) != ']')
			{
				if (*(pattern + 2) >= *pattern)
					ft_memset(&node->chars[(size_t) *pattern], 0x1, *(pattern + 2) - *pattern);
				pattern += 3;
				continue;
			}
		}
		node->chars[(size_t) *pattern] = 0x1;
		pattern += 1;
	}
	return (pattern);
}

static void
	_init_node(t_pattern_node *node)
{
	ft_memset(node->chars, 0, sizeof(node->chars)/sizeof(node->chars[0]));
	node->infinite = 0;
	node->invert = 0;
	node->next = NULL;
}

static void
	_add_node(t_pattern_node *head, t_pattern_node *node)
{
	while (head->next)
		head = head->next;
	head->next = node;
}

static t_pattern_node
	*_generate_pattern(const char *pattern, const char *info)
{
	t_pattern_node	*head;
	t_pattern_node	*current;

	head = NULL;
	while (pattern && *pattern)
	{
		current = sh_safe_malloc(sizeof(*head));
		_init_node(current);
		if (!head)
			head = current;
		else
			_add_node(head, current);
		if (*info & SH_PATTERN_ESCAPED) /* NOTE Info doesn't move together with the pattern string at the moment */
			current->chars[(size_t) *pattern] = 0x1;
		else if (*pattern == '[')
		{
			pattern = _process_brackets(pattern, current);
			continue;
		}
		else if (*pattern == '*')
		{
			ft_memset(&current->chars[0], 0xff, sizeof(head->chars) / sizeof(head->chars[0]));
			current->infinite = 1;
		}
		else if (*pattern == '?')
			ft_memset(current->chars, 1, sizeof(head->chars) / sizeof(head->chars[0]));
		else
			current->chars[(size_t) *pattern] = 0x1;
		pattern += 1;
	}
	return (head);
}

static int
	_is_match(char ch, t_pattern_node *const node)
{
	return (!!node->chars[(size_t) ch]);
}

static int
	_match_pattern(const char *str, t_pattern_node *node)
{
	int	cmp;

	if (*str && node && _is_match(*str, node))
	{
		if (node->infinite)
		{
			cmp = _match_pattern(str + 1, node);
			if (cmp)
				return (1);
		}
		return (_match_pattern(str + 1, node->next));
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

//case "abc" in "a[a-z]c") echo Yes;; esac
int
	match_pattern(const char *str, const char *pattern, const char *info)
{
	t_pattern_node	*head;

	head = _generate_pattern(pattern, info);
	sh_assert(head != NULL);
	return (_match_pattern(str, head));
}
