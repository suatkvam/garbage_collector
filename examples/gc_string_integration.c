/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_string_integration.c                            :+:      :+:    :+:   */
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
/*                    GC-AWARE STRING FUNCTIONS                               */
/* ************************************************************************** */

// strlen helper
static size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

/**
 * gc_strdup - Duplicate string using garbage collector
 * Instead of malloc(), uses gc_malloc()
 */
char	*gc_strdup(const char *s)
{
	char	*dup;
	size_t	len;
	size_t	i;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	dup = gc_malloc(len + 1);  // Use GC instead of malloc!
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

/**
 * gc_strjoin - Join two strings using garbage collector
 */
char	*gc_strjoin(const char *s1, const char *s2)
{
	char	*joined;
	size_t	len1;
	size_t	len2;
	size_t	i;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	joined = gc_malloc(len1 + len2 + 1);  // GC allocation!
	if (!joined)
		return (NULL);
	i = 0;
	while (i < len1)
	{
		joined[i] = s1[i];
		i++;
	}
	i = 0;
	while (i < len2)
	{
		joined[len1 + i] = s2[i];
		i++;
	}
	joined[len1 + len2] = '\0';
	return (joined);
}

/**
 * gc_substr - Extract substring using garbage collector
 */
char	*gc_substr(const char *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	s_len;
	size_t	i;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (gc_strdup(""));
	if (start + len > s_len)
		len = s_len - start;
	sub = gc_malloc(len + 1);  // GC allocation!
	if (!sub)
		return (NULL);
	i = 0;
	while (i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

/**
 * gc_split - Split string by delimiter using garbage collector
 */
static int	count_words(const char *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

char	**gc_split(const char *s, char c)
{
	char	**result;
	int		words;
	int		i;
	int		start;
	int		end;

	if (!s)
		return (NULL);
	words = count_words(s, c);
	result = gc_malloc(sizeof(char *) * (words + 1));  // GC allocation!
	if (!result)
		return (NULL);
	i = 0;
	start = 0;
	while (i < words)
	{
		while (s[start] == c)
			start++;
		end = start;
		while (s[end] && s[end] != c)
			end++;
		result[i] = gc_substr(s, start, end - start);  // Uses GC!
		start = end;
		i++;
	}
	result[i] = NULL;
	return (result);
}

/* ************************************************************************** */
/*                              MAIN EXAMPLE                                  */
/* ************************************************************************** */

int main(void)
{
	int		stack_var;
	char	*str1;
	char	*str2;
	char	*joined;
	char	*sub;
	char	**words;
	int		i;

	printf("=== GC String Integration Example ===\n\n");

	// Initialize GC
	collector_init(&stack_var);

	// Example 1: gc_strdup
	printf("1. Using gc_strdup:\n");
	str1 = gc_strdup("Hello, World!");
	printf("   Original: \"Hello, World!\"\n");
	printf("   Duplicated: \"%s\"\n\n", str1);

	// Example 2: gc_strjoin
	printf("2. Using gc_strjoin:\n");
	str2 = gc_strdup("Garbage Collector");
	joined = gc_strjoin(str1, " + ");
	joined = gc_strjoin(joined, str2);
	printf("   str1: \"%s\"\n", str1);
	printf("   str2: \"%s\"\n", str2);
	printf("   Joined: \"%s\"\n\n", joined);

	// Example 3: gc_substr
	printf("3. Using gc_substr:\n");
	sub = gc_substr("Extract this part", 8, 4);
	printf("   Original: \"Extract this part\"\n");
	printf("   Substring (8, 4): \"%s\"\n\n", sub);

	// Example 4: gc_split
	printf("4. Using gc_split:\n");
	words = gc_split("one,two,three,four,five", ',');
	printf("   Original: \"one,two,three,four,five\"\n");
	printf("   Split by ',':\n");
	i = 0;
	while (words[i])
	{
		printf("   [%d]: \"%s\"\n", i, words[i]);
		i++;
	}
	printf("\n");

	// Example 5: No need to free!
	printf("5. Memory Management:\n");
	printf("   ✓ No manual free() calls needed\n");
	printf("   ✓ All strings tracked by GC\n");
	printf("   ✓ Running gc_collect()...\n");
	gc_collect();
	printf("   ✓ Unused strings automatically freed\n\n");

	// Cleanup
	collector_close();
	printf("=== Example completed ===\n");
	return (0);
}
