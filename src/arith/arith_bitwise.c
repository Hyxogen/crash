#include "commander.h"
#include "minishell.h"

long
	arith_bitwise_or(const char *ptr, long lhs, long rhs, long c)
{
	(void) ptr;
	(void) c;
	return (lhs | rhs);
}

long
	arith_bitwise_and(const char *ptr, long lhs, long rhs, long c)
{
	(void) ptr;
	(void) c;
	return (lhs & rhs);
}

long
	arith_bitwise_xor(const char *ptr, long lhs, long rhs, long c)
{
	(void) ptr;
	(void) c;
	return (lhs ^ rhs);
}
