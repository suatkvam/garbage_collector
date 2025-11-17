/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_state.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 17:12:56 by akivam            #+#    #+#             */
/*   Updated: 2025/11/17 17:12:57 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <internal_collecter.h>

t_collecter	**get_gc_head(void)
{
	static t_collecter	*head = NULL;

	return (&head);
}

void	**get_gc_stack_start(void)
{
	static void *stack_statrt = NULL;
	
	return (&stack_statrt);
}