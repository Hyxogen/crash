#include <stdio.h>

#include <fcntl.h>
#include <limits.h>
#include <errno.h>

int main(void) {
	int	i;
	int count;
	
	count = 0;
	for (i = 0; i < OPEN_MAX; i++) {
		if (fcntl(i, F_GETFD) < 0) {
			if (errno != EBADF)
				fprintf(stderr, "Error\n");
		} else {
			printf("%d OPEN; ", i);
			count++;
		}
	}
	printf("\n%d/%d open\n", count, OPEN_MAX);
	return 0;
}