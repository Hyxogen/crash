#include "memory.h"

#define SH_PATTERN_IGNORE	0b00010000
#define SH_PATTERN_ESCAPED	0b00100000

static short
	*_prepare_pattern(const char *pattern)
{

}

int
	match_pattern(const char *pattern, const char *str)
{
	short	*processed_pattern;

	processed_pattern = _prepare_pattern(pattern);
}