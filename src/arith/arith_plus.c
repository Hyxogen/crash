#include "minishell.h"

#include "commander.h"


#include <stdio.h>

long
	arith_plus(const char *str, long lhs, long rhs, long c)
{
	(void) str;
	(void) c;
	return (lhs + rhs);
	fprintf(stderr, "arith_plus\n");
}

long
	arith_plus_eq(const char *str, long lhs, long rhs, long c)
{
	char	sum_str[32];
	long	sum;

	(void) c;
	sum = lhs + rhs;
	sh_ltoa(sum, sum_str, 32);
	sh_setenv(str, sum_str, 0);
	return (sum);
}
