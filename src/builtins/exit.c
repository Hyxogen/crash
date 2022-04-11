#include "commander.h"

#include <libft.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int
	sh_atol(const char *str, long *v)
{
	int		s;

	while ((*str >= 9 && *str <= 13) || *str == 32)
		str += 1;
	s = 1;
	if (*str == '-')
		s = -s;
	if (*str == '-' || *str == '+')
		str += 1;
	*v = 0;
	if (*str == '\0')
		return (-1);
	while (ft_isdigit(*str))
	{
		if (*v > LONG_MAX / 10 || (s > 0 && *v * 10 > LONG_MAX - (*str - '0')))
			return (-1);
		if (*v < LONG_MIN / 10 || (s < 0 && *v * 10 < LONG_MIN + (*str - '0')))
			return (-1);
		*v = *v * 10 + (*str - '0') * s;
		str += 1;
	}
	if (*str != '\0')
		return (-1);
	return (0);
}

int
	sh_exit(int argc, char **argv)
{
	long	ex_code;

	if (argc <= 1)
		exit(0);
	if (argc >= 3)
	{
		sh_err2("exit", "too many arguments");
		exit(0);
	}
	if (sh_atol(argv[1], &ex_code) < 0)
	{
		sh_err2("exit", "numeric argument required");
		exit(0);
	}
	exit(ex_code);
	return (-1);
}
