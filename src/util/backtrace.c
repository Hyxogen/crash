/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   backtrace.c                                      -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:46 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:46 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include <execinfo.h>
#include <stdlib.h>

#ifdef SH_BACKTRACE

void
	sh_backtrace(int count)
{
	void	**buffer;

	buffer = malloc(sizeof(*buffer) * (count + 1));
	count = backtrace(buffer, count + 1);
	backtrace_symbols_fd(buffer + 1, count - 1, STDERR_FILENO);
	free(buffer);
}

#else

void
	sh_backtrace(int count)
{
	(void) count;
}

#endif
