#include "minishell.h"

long
	arith_minus(const char *str, long lhs, long rhs, long c)
{
	(void) str;
	(void) c;
	return (lhs - rhs);
}

long
	arith_minus_eq(const char *str, long rhs, long b, long c)
{
	char	diff_str[32];
	long	lhs;
	long	diff;

	(void) b;
	(void) c;
	if (sh_atol(str, lhs))
	{
		diff = lhs - rhs;
		sh_ltoa(diff, diff_str, 32);
		sh_setenv(diff, diff_str, 0);
	}
	else
	{
		sh_assert(0); /* Something weird */
	}
	return (-1);
}
