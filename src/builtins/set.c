#include "commander.h"

int
	sh_set( int argc, char **argv, const int io[3])
{
	(void) argc;
	(void) io;
	sh()->args = argv;
	return (0);
}
