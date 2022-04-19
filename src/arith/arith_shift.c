#include "commander.h"
#include "minishell.h"

long
	arith_shift_left(const char *ptr, long lhs, long rhs, long c)
{
	(void) ptr;
	(void) c;
	return (lhs << rhs);
}

long
	arith_shift_left_eq(const char *str, long lhs, long rhs, long c)
{
	char	result_str[32];
	long	result;

	(void) c;
	result = lhs << rhs;
	sh_ltoa(result, result_str, 32);
	sh_setenv(str, result_str, 0);
	return (result);
}

long
	arith_shift_right(const char *ptr, long lhs, long rhs, long c)
{
	(void) ptr;
	(void) c;
	return (lhs >> rhs);
}

long
	arith_shift_right_eq(const char *str, long lhs, long rhs, long c)
{
	char	result_str[32];
	long	result;

	(void) c;
	result = lhs >> rhs;
	sh_ltoa(result, result_str, 32);
	sh_setenv(str, result_str, 0);
	return (result);
}
