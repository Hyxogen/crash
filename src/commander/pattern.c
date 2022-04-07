#include "commander.h"

#include "memory.h"

static const char
	*_expect_char(const char *str, char c)
{
	if (*str == c)
		return (str + 1);
	return (NULL);
}

static const char
	*_process_col_class(const char *pattern, t_pattern *node)
{
	pattern = _expect_char(pattern, '[');
	pattern = _expect_char(pattern, '.');
	if (pattern && *pattern && *(pattern + 1) == '.' && *(pattern + 2) == ']')
	{
		node->chars[*pattern] = 0x1;
		return (pattern + 3);
	}
	return (NULL);
}

static const char
	*_process_eq_class(const char *pattern, t_pattern_node *node)
{
	pattern = _expect_char(pattern, "[");
	pattern = _expect_char(pattern, "=");
	if (pattern && *pattern && *(pattern + 1) == "=" && *(pattern + 2) == ']')
	{
		/* TODO add all equivalent characters */
		node->chars[*pattern] = 0x1;
		return (pattern + 3);
	}
	return (NULL);
}

static void
	_enable_chars(t_pattern_node *node, int (*isfunc)(int))
{

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
			else if (*(pattern + 1) == ":")
			{
				pattern = _process_char_class(pattern, node);
				continue;
			}
		}
		if (*(pattern + 1) == '-')
		{
			if (!*(pattern + 2))
				return (NULL);
			if (*(pattern + 2) != ']')
			{
				if (*(pattern + 2) < *pattern)
					pattern += 2;
				else
					ft_memset(&node->chars[*pattern], 0x1, *(pattern + 2) - *pattern)
				continue;
			}
		}
		node->chars[*pattern] = 0x1;
		pattern += 1;
	}
	return (pattern);
}

t_pattern_node
	*_generate_pattern(const char *pattern)
{
	t_pattern_node	*head;
	t_pattern_node	*current;
	int				escaped;
	size_t			index;
}

int
	match_pattern(const char *pattern, const char *str)
{
	short	*processed_pattern;

	processed_pattern = _prepare_pattern(pattern);
}