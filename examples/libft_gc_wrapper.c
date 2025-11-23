/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_gc_wrapper.c                                 :+:      :+:    :+:   */
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
/*              WRAPPER FUNCTIONS - Convert libft to GC versions             */
/* ************************************************************************** */

/**
 * GC Wrapper Pattern:
 * 1. Replace malloc() with gc_malloc()
 * 2. Keep all other logic the same
 * 3. Don't call free() - GC handles it
 */

// Example: Your original ft_strdup
char	*ft_strdup_original(const char *s)
{
	char	*dup;
	size_t	len;
	size_t	i;

	len = 0;
	while (s[len])
		len++;
	dup = malloc(len + 1);  // ← Uses malloc
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

// GC version - only change malloc to gc_malloc!
char	*ft_strdup_gc(const char *s)
{
	char	*dup;
	size_t	len;
	size_t	i;

	len = 0;
	while (s[len])
		len++;
	dup = gc_malloc(len + 1);  // ← Uses gc_malloc instead!
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

// Another example: ft_itoa with GC
char	*ft_itoa_gc(int n)
{
	char	*str;
	long	num;
	int		len;
	int		sign;

	num = n;
	sign = (n < 0) ? 1 : 0;
	len = (n <= 0) ? 1 : 0;
	if (n < 0)
		num = -num;
	while (n)
	{
		n /= 10;
		len++;
	}
	str = gc_malloc(len + sign + 1);  // ← GC allocation
	if (!str)
		return (NULL);
	str[len + sign] = '\0';
	while (len--)
	{
		str[len + sign] = (num % 10) + '0';
		num /= 10;
	}
	if (sign)
		str[0] = '-';
	return (str);
}

/* ************************************************************************** */
/*                      USING EXISTING LIBFT WITH GC                         */
/* ************************************************************************** */

/**
 * If you have existing libft that uses malloc,
 * you can create a wrapper layer:
 */

// Assuming you have these libft functions:
// char *ft_strdup(const char *s);
// char *ft_strjoin(const char *s1, const char *s2);

// Create GC wrappers that copy the result
char	*gc_wrap_strdup(const char *s)
{
	char	*original;
	char	*gc_copy;
	size_t	len;
	size_t	i;

	// Use your existing libft function
	original = ft_strdup_original(s);
	if (!original)
		return (NULL);
	
	// Copy to GC-managed memory
	len = 0;
	while (original[len])
		len++;
	gc_copy = gc_malloc(len + 1);
	if (!gc_copy)
	{
		free(original);  // Free the malloc'd version
		return (NULL);
	}
	
	i = 0;
	while (i <= len)
	{
		gc_copy[i] = original[i];
		i++;
	}
	
	free(original);  // Free the malloc'd version
	return (gc_copy);  // Return GC version
}

/* ************************************************************************** */
/*                              MAIN EXAMPLE                                  */
/* ************************************************************************** */

int main(void)
{
	int		stack_var;
	char	*s1;
	char	*s2;
	char	*num_str;

	printf("=== Libft + GC Integration Example ===\n\n");

	collector_init(&stack_var);

	// Method 1: Direct GC versions
	printf("1. Using direct GC versions:\n");
	s1 = ft_strdup_gc("Hello from GC!");
	printf("   ft_strdup_gc: \"%s\"\n\n", s1);

	// Method 2: Wrapped libft functions
	printf("2. Using wrapped libft functions:\n");
	s2 = gc_wrap_strdup("Wrapped string");
	printf("   gc_wrap_strdup: \"%s\"\n\n", s2);

	// Method 3: GC-aware utility functions
	printf("3. Using GC-aware utilities:\n");
	num_str = ft_itoa_gc(42);
	printf("   ft_itoa_gc(42): \"%s\"\n\n", num_str);

	// All strings are automatically managed
	printf("4. Automatic memory management:\n");
	printf("   ✓ No need to free s1, s2, num_str\n");
	printf("   ✓ All tracked by GC\n\n");

	collector_close();
	printf("=== Example completed ===\n");
	return (0);
}
