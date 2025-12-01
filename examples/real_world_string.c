/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   real_world_string.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 13:15:03 by akivam            #+#    #+#             */
/*   Updated: 2025/12/01 13:15:04 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../garbage_collector.h"
#include <stdio.h>

/* ************************************************************************** */
/*                         STRING UTILITY LIBRARY                             */
/* ************************************************************************** */

typedef struct s_string
{
	char		*data;
	size_t		length;
}				t_string;

// Helper function
static size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

/**
 * Create a new GC-managed string
 */
t_string	*string_new(const char *str)
{
	t_string	*s;
	size_t		i;

	s = gc_malloc(sizeof(t_string));
	if (!s)
		return (NULL);
	s->length = ft_strlen(str);
	s->data = gc_malloc(s->length + 1);
	if (!s->data)
		return (NULL);
	i = 0;
	while (i < s->length)
	{
		s->data[i] = str[i];
		i++;
	}
	s->data[i] = '\0';
	return (s);
}

/**
 * Concatenate two strings
 */
t_string	*string_concat(t_string *s1, t_string *s2)
{
	t_string	*result;
	size_t		i;

	result = gc_malloc(sizeof(t_string));
	if (!result)
		return (NULL);
	result->length = s1->length + s2->length;
	result->data = gc_malloc(result->length + 1);
	if (!result->data)
		return (NULL);
	i = 0;
	while (i < s1->length)
	{
		result->data[i] = s1->data[i];
		i++;
	}
	i = 0;
	while (i < s2->length)
	{
		result->data[s1->length + i] = s2->data[i];
		i++;
	}
	result->data[result->length] = '\0';
	return (result);
}

/**
 * Convert string to uppercase
 */
t_string	*string_toupper(t_string *s)
{
	t_string	*result;
	size_t		i;

	result = gc_malloc(sizeof(t_string));
	if (!result)
		return (NULL);
	result->length = s->length;
	result->data = gc_malloc(result->length + 1);
	if (!result->data)
		return (NULL);
	i = 0;
	while (i < s->length)
	{
		if (s->data[i] >= 'a' && s->data[i] <= 'z')
			result->data[i] = s->data[i] - 32;
		else
			result->data[i] = s->data[i];
		i++;
	}
	result->data[i] = '\0';
	return (result);
}

/**
 * Trim whitespace
 */
t_string	*string_trim(t_string *s)
{
	t_string	*result;
	size_t		start;
	size_t		end;
	size_t		i;

	start = 0;
	while (start < s->length && (s->data[start] == ' ' || s->data[start] == '\t'
			|| s->data[start] == '\n'))
		start++;
	end = s->length;
	while (end > start && (s->data[end - 1] == ' ' || s->data[end - 1] == '\t'
			|| s->data[end - 1] == '\n'))
		end--;
	result = gc_malloc(sizeof(t_string));
	if (!result)
		return (NULL);
	result->length = end - start;
	result->data = gc_malloc(result->length + 1);
	if (!result->data)
		return (NULL);
	i = 0;
	while (i < result->length)
	{
		result->data[i] = s->data[start + i];
		i++;
	}
	result->data[i] = '\0';
	return (result);
}

/* ************************************************************************** */
/*                              MAIN EXAMPLE                                  */
/* ************************************************************************** */

int	main(void)
{
	int			stack_var;
	t_string	*s1;
	t_string	*s2;
	t_string	*s3;
	t_string	*upper;
	t_string	*trimmed;
	t_string	*with_spaces;
	t_string	*greeting;
	t_string	*name;
	t_string	*combined;
	t_string	*final;

	printf("=== Real World String Example ===\n\n");
	collector_init(&stack_var);
	// Example 1: Create strings
	printf("1. Creating strings:\n");
	s1 = string_new("Hello");
	s2 = string_new("World");
	printf("   s1: \"%s\" (length: %zu)\n", s1->data, s1->length);
	printf("   s2: \"%s\" (length: %zu)\n\n", s2->data, s2->length);
	// Example 2: Concatenate
	printf("2. Concatenating strings:\n");
	s3 = string_concat(s1, s2);
	printf("   Result: \"%s\" (length: %zu)\n\n", s3->data, s3->length);
	// Example 3: Uppercase
	printf("3. Converting to uppercase:\n");
	upper = string_toupper(s3);
	printf("   Original: \"%s\"\n", s3->data);
	printf("   Uppercase: \"%s\"\n\n", upper->data);
	// Example 4: Trimming
	printf("4. Trimming whitespace:\n");
	with_spaces = string_new("   trim me   ");
	printf("   Original: \"%s\" (length: %zu)\n", with_spaces->data,
		with_spaces->length);
	trimmed = string_trim(with_spaces);
	printf("   Trimmed: \"%s\" (length: %zu)\n\n", trimmed->data,
		trimmed->length);
	// Example 5: Complex operations
	printf("5. Complex string operations:\n");
	greeting = string_new("hello, ");
	name = string_new("garbage collector");
	combined = string_concat(greeting, name);
	final = string_toupper(combined);
	printf("   Final: \"%s\"\n\n", final->data);
	// Example 6: Memory management
	printf("6. Memory management:\n");
	printf("   ✓ All strings automatically managed\n");
	printf("   ✓ No memory leaks\n");
	printf("   ✓ No manual free() calls\n\n");
	// Cleanup
	gc_collect();
	printf("   Garbage collection completed\n");
	collector_close();
	printf("\n=== Example completed ===\n");
	return (0);
}
