#include <stdio.h>

#include <fcntl.h>
#include <limits.h>
#include <errno.h>

int
	main(void)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (i < OPEN_MAX)
	{
		if (fcntl(i, F_GETFD) < 0)
		{
			if (errno != EBADF)
				fprintf(stderr, "Error\n");
		}
		else
		{
			printf("%d OPEN; ", i);
			count++;
		}
		i++;
	}
	printf("\n%d/%d open\n", count, OPEN_MAX);
	return (0);
}
