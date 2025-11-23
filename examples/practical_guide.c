/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   practical_guide.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 22:29:44 by akivam              #+#    #+#             */
/*   Updated: 2025/11/23 22:29:44 by akivam             ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../garbage_collector.h"
#include <stdio.h>

/* ************************************************************************** */
/*                    HOW TO CONVERT YOUR FUNCTIONS                          */
/* ************************************************************************** */

/*
 * STEP 1: Find all malloc() calls
 * STEP 2: Replace with gc_malloc()
 * STEP 3: Remove all free() calls
 * STEP 4: Done!
 */

// ============================================================================
// BEFORE: Original function with malloc
// ============================================================================

char	*my_strdup_before(const char *s)
{
	char	*dup;
	size_t	len;
	size_t	i;

	len = 0;
	while (s[len])
		len++;
	dup = malloc(len + 1);        // ← malloc here
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

void	usage_before(void)
{
	char	*str;

	str = my_strdup_before("test");
	printf("%s\n", str);
	free(str);                    // ← must free manually
}

// ============================================================================
// AFTER: GC-aware version
// ============================================================================

char	*my_strdup_after(const char *s)
{
	char	*dup;
	size_t	len;
	size_t	i;

	len = 0;
	while (s[len])
		len++;
	dup = gc_malloc(len + 1);     // ← gc_malloc instead!
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

void	usage_after(void)
{
	char	*str;

	str = my_strdup_after("test");
	printf("%s\n", str);
	// No free() needed!         // ← GC handles it!
}

/* ************************************************************************** */
/*                    COMMON PATTERNS                                         */
/* ************************************************************************** */

// Pattern 1: Simple string duplication
char	*gc_pattern_strdup(const char *s)
{
	char	*dup;
	int		i;

	i = 0;
	while (s[i])
		i++;
	dup = gc_malloc(i + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

// Pattern 2: String concatenation
char	*gc_pattern_strjoin(const char *s1, const char *s2)
{
	char	*result;
	int		len1;
	int		len2;
	int		i;

	len1 = 0;
	while (s1[len1])
		len1++;
	len2 = 0;
	while (s2[len2])
		len2++;
	result = gc_malloc(len1 + len2 + 1);
	if (!result)
		return (NULL);
	i = -1;
	while (++i < len1)
		result[i] = s1[i];
	i = -1;
	while (++i < len2)
		result[len1 + i] = s2[i];
	result[len1 + len2] = '\0';
	return (result);
}

// Pattern 3: Array allocation
int	*gc_pattern_array(int size)
{
	int	*arr;
	int	i;

	arr = gc_malloc(sizeof(int) * size);
	if (!arr)
		return (NULL);
	i = 0;
	while (i < size)
	{
		arr[i] = 0;
		i++;
	}
	return (arr);
}

// Pattern 4: Structure allocation
typedef struct s_data
{
	int		value;
	char	*name;
}	t_data;

t_data	*gc_pattern_struct(int value, const char *name)
{
	t_data	*data;

	data = gc_malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->value = value;
	data->name = gc_pattern_strdup(name);
	return (data);
}

/* ************************************************************************** */
/*                              MAIN EXAMPLE                                  */
/* ************************************************************************** */

int main(void)
{
	int		stack_var;
	char	*str1;
	char	*str2;
	int		*arr;
	t_data	*data;

	printf("=== Practical Integration Guide ===\n\n");

	collector_init(&stack_var);

	// 1. String duplication
	printf("1. String duplication:\n");
	str1 = gc_pattern_strdup("Hello, GC!");
	printf("   Result: %s\n\n", str1);

	// 2. String joining
	printf("2. String joining:\n");
	str2 = gc_pattern_strjoin("Part1", "Part2");
	printf("   Result: %s\n\n", str2);

	// 3. Array allocation
	printf("3. Array allocation:\n");
	arr = gc_pattern_array(5);
	arr[0] = 10;
	arr[1] = 20;
	arr[2] = 30;
	printf("   Array: %d, %d, %d\n\n", arr[0], arr[1], arr[2]);

	// 4. Structure allocation
	printf("4. Structure allocation:\n");
	data = gc_pattern_struct(42, "Test Data");
	printf("   Value: %d\n", data->value);
	printf("   Name: %s\n\n", data->name);

	// 5. Everything is auto-managed!
	printf("5. Automatic cleanup:\n");
	printf("   ✓ No free() calls needed\n");
	printf("   ✓ All memory tracked by GC\n");
	printf("   ✓ Safe from memory leaks\n\n");

	collector_close();
	printf("=== Guide completed ===\n");
	return (0);
}
