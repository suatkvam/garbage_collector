/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_wrap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 22:29:43 by akivam              #+#    #+#             */
/*   Updated: 2025/11/28 20:25:14 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GC_WRAP_H
# define GC_WRAP_H

/**
 * Optional manual control API
 * If not used, GC auto-initializes
 */

void	gc_init_manual(void *stack_start);
void	gc_collect_manual(void);
void	gc_cleanup_manual(void);

// Convenience macros
# define GC_INIT(var)     gc_init_manual(&var)
# define GC_COLLECT()     gc_collect_manual()
# define GC_CLEANUP()     gc_cleanup_manual()

#endif
