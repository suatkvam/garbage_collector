/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 16:57:16 by akivam            #+#    #+#             */
/*   Updated: 2025/11/17 16:57:17 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collecter.h"

void	*gc_malloc(size_t size)
{
	t_collecter	*new_header;
	t_collecter	**head_ptr;

	if (size == 0)
		return (NULL);
	new_header = (t_collecter *)malloc(sizeof(t_collecter) + size);
	if (!new_header)
		return (NULL);
	new_header->is_marked = 0;
	new_header->size = size;
	*head_ptr = new_header;
	return ((void *)(new_header + 1));
}
