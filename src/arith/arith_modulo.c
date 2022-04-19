#include "minishell.h"

long
	arith_modulo(const char *str, long dividend, long divisor, long c)
{
	(void) str;
	(void) c;
	return (dividend % divisor);
}

long
	arith_modulo_eq(const char *str, long lhs, long rhs, long c)
{
	char	remainder_str[32];
	long	remainder;

	(void) c;
	remainder = lhs % rhs;
	sh_ltoa(remainder, remainder_str, 32);
	sh_setenv(str, remainder_str, 0);
	return (remainder);
}
