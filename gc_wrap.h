/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_wrap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 12:44:01 by akivam            #+#    #+#             */
/*   Updated: 2025/12/01 12:44:04 by akivam           ###   ########.fr       */
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
