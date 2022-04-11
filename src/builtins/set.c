#include "commander.h"

int
	sh_set(int argc, char **argv)
{
	(void) argc;
	sh()->args = argv;
	return (0);
}
