#include "minishell.h"

long
	arith_multiply(const char *str, long lhs, long rhs, long c)
{
	(void) str;
	(void) c;
	return (lhs * rhs);
}

long
	arith_multiply_eq(const char *str, long rhs, long b, long c)
{
	char	product_str[32];
	long	lhs;
	long	product;

	(void) b;
	(void) c;
	if (sh_atol(str, lhs))
	{
		product = lhs * rhs;
		sh_ltoa(product, product_str, 32);
		sh_setenv(product, product_str, 0);
	}
	else
	{
		sh_assert(0); /* Something weird */
	}
	return (-1);
}
