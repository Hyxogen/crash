#include "minishell.h"

long
	arith_divide(const char *str, long dividend, long divisor, long c)
{
	(void) str;
	(void) c;
	if (divisor == 0)
	{
		sh_err1("division by zero");
		return (0);
	}
	return (dividend / divisor);
}

long
	arith_divide_eq(const char *str, long lhs, long rhs, long c)
{
	char	quotient_str[32];
	long	quotient;

	(void) c;
	quotient = lhs % rhs;
	sh_ltoa(quotient, quotient_str, 32);
	sh_setenv(str, quotient_str, 0);
	return (quotient);
}
