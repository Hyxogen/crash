#include "minishell.h"

long
	arith_divide(const char *str, long dividend, long divisor, long c)
{
	(void) str;
	(void) c;
	return (dividend / divisor);
}

long
	arith_divide_eq(const char *str, long divisor, long b, long c)
{
	char	quotient_str[32];
	long	dividend;
	long	quotient;

	(void) c;
	if (sh_atol(str, dividend))
	{
		quotient = dividend / divisor ;
		sh_ltoa(quotient, quotient_str, 32);
		sh_setenv(quotient, quotient_str, 0);
	}
	else
	{
		sh_assert(0); /* Something weird */
	}
	return (-1);
}
