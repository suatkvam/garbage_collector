/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_collecter.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 16:57:17 by akivam            #+#    #+#             */
/*   Updated: 2025/11/17 21:02:46 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//! private header
#ifndef INTERNAL_COLLECTER_H
# define INTERNAL_COLLECTER_H

# include "garbage_collecter.h"
# include <stdlib.h>

t_collecter	**get_gc_head(void);
void		**get_gc_stack_start(void);

void		gc_collect(void);

// core GC logic functions
void		gc_mark(void);
void		gc_sweep(void);
t_collecter	*get_header_from_ptr(void *ptr);

// gc_mark helper functions
int			is_valid_pointer(void *ptr);
void		mark_memory_region(void *start, size_t size);
void		mark_pointer(void *ptr);
void		mark_from_stack(void);

// memory manipulation functions
void		gc_bzero(void *s, size_t n);
void		*gc_memcpy(void *dest, const void *src, size_t n);
int			gc_memcmp(const void *s1, const void *s2, size_t n);
void		*gc_memchr(const void *s1, int c, size_t n);
void		*gc_memmove(void *dest, const void *src, size_t n);
void		*gc_memset(void *s, int c, size_t n);

#endif
