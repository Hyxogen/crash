#include "commander.h"

long
	arith_less_than(const char *str, long lhs, long rhs, long c)
{
	(void) str;
	(void) c;
	return (lhs < rhs);
}

long
	arith_less_than_or_eq(const char *str, long lhs, long rhs, long c)
{
	(void) str;
	(void) c;
	return (lhs <= rhs);
}

long
	arith_greater_than(const char *str, long lhs, long rhs, long c)
{
	(void) str;
	(void) c;
	return (lhs > rhs);
}

long
	arith_greater_than_or_eq(const char *str, long lhs, long rhs, long c)
{
	(void) str;
	(void) c;
	return (lhs >= rhs);
}
