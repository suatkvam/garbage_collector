/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_malloc.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 22:29:43 by akivam              #+#    #+#           */
/*   Updated: 2025/11/23 22:29:43 by akivam             ###   ########.tr     */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNAL_MALLOC_H
# define INTERNAL_MALLOC_H

# include <stddef.h>

/**
 * INTERNAL MALLOC MACROS
 * 
 * These macros allow GC to use real malloc internally
 * without triggering --wrap recursion.
 * 
 * When compiled with -DUSE_GC_WRAP:
 *   - GC_INTERNAL_MALLOC uses __real_malloc
 *   - Prevents infinite recursion
 * 
 * Without flag:
 *   - Uses normal malloc
 */

# ifdef USE_GC_WRAP

// Declare real functions (provided by linker with --wrap)
extern void	*__real_malloc(size_t size);
extern void	*__real_calloc(size_t nmemb, size_t size);
extern void	*__real_realloc(void *ptr, size_t size);
extern void	__real_free(void *ptr);

// Macros for GC internal use (with safer parentheses)
#  define GC_INTERNAL_MALLOC(size)        __real_malloc((size))
#  define GC_INTERNAL_CALLOC(n, size)     __real_calloc((n), (size))
#  define GC_INTERNAL_REALLOC(ptr, size)  __real_realloc((ptr), (size))
#  define GC_INTERNAL_FREE(ptr)           __real_free((ptr))

# else

// Normal mode: use standard functions
#  include <stdlib.h>

#  define GC_INTERNAL_MALLOC(size)        malloc((size))
#  define GC_INTERNAL_CALLOC(n, size)     calloc((n), (size))
#  define GC_INTERNAL_REALLOC(ptr, size)  realloc((ptr), (size))
#  define GC_INTERNAL_FREE(ptr)           free((ptr))

# endif

#endif
