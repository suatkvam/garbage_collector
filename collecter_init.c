/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collecter_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 17:30:39 by akivam            #+#    #+#             */
/*   Updated: 2025/11/17 17:30:40 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collecter.h"

void collecter_init (void *stack_start)
{
	void **stack_ptr;

	stack_ptr = get_gc_stack_start();
	*stack_ptr = stack_start;
}