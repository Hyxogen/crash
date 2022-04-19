#include "minishell.h"

long
	arith_modulo(const char *str, long dividend, long divisor, long c)
{
	(void) str;
	(void) c;
	return (dividend % divisor);
}

long
	arith_modulo_eq(const char *str, long divisor, long b, long c)
{
	char	remainder_str[32];
	long	dividend;
	long	remainder;

	(void) c;
	if (sh_atol(str, dividend))
	{
		remainder = dividend % divisor ;
		sh_ltoa(remainder, remainder_str, 32);
		sh_setenv(remainder, remainder_str, 0);
	}
	else
	{
		sh_assert(0); /* Something weird */
	}
	return (-1);
}
