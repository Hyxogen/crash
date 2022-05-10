/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   memory.h                                         -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:58 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:58 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_H
# define MEMORY_H

# include <sys/types.h>

void	*sh_safe_malloc(size_t size);
void	*sh_safe_realloc(void *ptr, size_t old_size, size_t new_size);
void	*sh_safe_reallog(void *ptr, size_t old_size, size_t new_size);

#endif
