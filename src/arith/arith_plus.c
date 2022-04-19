#include "minishell.h"

long
	arith_plus(const char *str, long lhs, long rhs, long c)
{
	(void) str;
	(void) c;
	return (lhs + rhs);
}

// long
// 	arith_decrement(const char *str, long a, long b, long c)
// {
// 	long	before;
// }

// long
// 	arith_plus_eq(const char *str, long rhs, long b, long c)
// {
// 	char	sum_str[32];
// 	long	lhs;
// 	long	sum;

// 	(void) b;
// 	(void) c;
// 	if (sh_atol(str, lhs))
// 	{
// 		sum = lhs + rhs;
// 		sh_ltoa(sum, sum_str, 32);
// 		sh_setenv(str, sum_str, 0);
// 	}
// 	else
// 	{
// 		sh_assert(0); /* Something weird */
// 	}
// 	return (-1);
// }
