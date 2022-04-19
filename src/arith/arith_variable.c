#include "minishell"

long
	arith_get_val(const char *key)
{
	const char	*value_str;
	long		value;

	value_str = sh_getenv(key, "0");
	if (sh_atol(value_str, &value))
		return (value);
	sh_assert(0);
	return (0);	
}