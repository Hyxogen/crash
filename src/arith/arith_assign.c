#include "minishell.h"
#include "commander.h"

long
	arith_assign(const char *str, long old_value, long value, long c)
{
	char	value_str[32];

	(void) old_value;
	(void) c;
	sh_ltoa(value, value_str, 32);
	sh_setenv(str, value_str, 0);
	return (value);
}
