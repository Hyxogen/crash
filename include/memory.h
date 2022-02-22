/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   memory.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/21 15:13:24 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/21 15:17:37 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_H
# define MEMORY_H

# include <sys/types.h>

void	*sh_safe_malloc(size_t size);
void	*sh_safe_realloc(void *ptr, size_t old_size, size_t new_size);

#endif
