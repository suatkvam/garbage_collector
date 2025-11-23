/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   examples_no_changes_example_Version2.c             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 22:29:44 by akivam              #+#    #+#             */
/*   Updated: 2025/11/23 22:29:44 by akivam             ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * NO CODE CHANGES EXAMPLE
 * 
 * This is your existing code - works exactly as-is!
 * 
 * Compile with:
 *   make normal  - Uses standard malloc/free (you manage memory)
 *   make gc      - Uses garbage collector (automatic management)
 * 
 * The --wrap linker flag intercepts malloc/free automatically!
 * NO source code modifications needed!
 */

/* ************************************************************************** */
/*                      YOUR EXISTING STRING FUNCTIONS                        */
/* ************************************************************************** */

static size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

char	*ft_strdup(const char *s)
{
	char	*dup;
	size_t	len;
	size_t	i;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	dup = malloc(len + 1);  // ← Automatically intercepted!
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

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*joined;
	size_t	len1;
	size_t	len2;
	size_t	i;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	joined = malloc(len1 + len2 + 1);  // ← Intercepted!
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

/* ************************************************************************** */
/*                      YOUR EXISTING LINKED LIST                             */
/* ************************************************************************** */

typedef struct s_node
{
	int				value;
	struct s_node	*next;
}	t_node;

t_node	*create_node(int value)
{
	t_node	*node;

	node = malloc(sizeof(t_node));  // ← Intercepted!
	if (!node)
		return (NULL);
	node->value = value;
	node->next = NULL;
	return (node);
}

void	print_list(t_node *head)
{
	t_node	*current;

	current = head;
	printf("List: ");
	while (current)
	{
		printf("%d", current->value);
		if (current->next)
			printf(" -> ");
		current = current->next;
	}
	printf("\n");
}

void	free_list(t_node *head)
{
	t_node	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head);  // ← In GC mode: no-op; In normal mode: frees
		head = tmp;
	}
}

/* ************************************************************************** */
/*                              MAIN PROGRAM                                  */
/* ************************************************************************** */

int main(void)
{
	char	*str1;
	char	*str2;
	char	*joined;
	t_node	*list;
	int		*array;
	int		i;

	printf("╔════════════════════════════════════════════════════════╗\n");
	printf("║     No Code Changes Required - Example                ║\n");
	printf("╚════════════════════════════════════════════════════════╝\n\n");

#ifdef USE_GC_WRAP
	printf("Mode: GARBAGE COLLECTOR (automatic memory management)\n\n");
#else
	printf("Mode: STANDARD MALLOC/FREE (manual memory management)\n\n");
#endif

	// ========== Example 1: String Operations ==========
	printf("1. String Operations:\n");
	printf("   Creating strings with ft_strdup...\n");
	str1 = ft_strdup("Hello");
	str2 = ft_strdup("World");
	
	printf("   str1: \"%s\"\n", str1);
	printf("   str2: \"%s\"\n", str2);
	
	printf("   Joining strings...\n");
	joined = ft_strjoin(str1, str2);
	printf("   joined: \"%s\"\n\n", joined);

	// ========== Example 2: Linked List ==========
	printf("2. Linked List:\n");
	printf("   Creating list: 1 -> 2 -> 3 -> 4 -> 5\n");
	list = create_node(1);
	list->next = create_node(2);
	list->next->next = create_node(3);
	list->next->next->next = create_node(4);
	list->next->next->next->next = create_node(5);
	
	printf("   ");
	print_list(list);
	printf("\n");

	// ========== Example 3: Array Allocation ==========
	printf("3. Array Allocation:\n");
	printf("   Allocating array of 10 integers...\n");
	array = malloc(sizeof(int) * 10);  // ← Intercepted!
	if (array)
	{
		i = 0;
		while (i < 10)
		{
			array[i] = i * 2;
			i++;
		}
		printf("   Array: [");
		i = 0;
		while (i < 10)
		{
			printf("%d", array[i]);
			if (i < 9)
				printf(", ");
			i++;
		}
		printf("]\n\n");
	}

	// ========== Example 4: Multiple Temporary Allocations ==========
	printf("4. Temporary Allocations:\n");
	printf("   Creating 100 temporary allocations...\n");
	i = 0;
	while (i < 100)
	{
		void *temp = malloc(64);
		// These are temporary, will be collected in GC mode
		(void)temp;
		i++;
	}
	printf("   Done!\n\n");

	// ========== Memory Management ==========
	printf("5. Memory Management:\n");

#ifdef USE_GC_WRAP
	printf("   ✓ GC Mode: All allocations tracked\n");
	printf("   ✓ No manual free() needed\n");
	printf("   ✓ Memory will be cleaned up automatically\n");
	printf("   ✓ Calling free() is safe but unnecessary (no-op)\n\n");
	
	// In GC mode, these free calls are no-ops
	free(str1);
	free(str2);
	free(joined);
	free_list(list);
	free(array);
	
	printf("   Called free() on all allocations (no-ops in GC mode)\n");
#else
	printf("   ✓ Normal Mode: Manual memory management\n");
	printf("   ✓ Must call free() for each allocation\n");
	printf("   ✓ Freeing all allocations...\n\n");
	
	// In normal mode, must free manually
	free(str1);
	free(str2);
	free(joined);
	free_list(list);
	free(array);
	
	printf("   All memory freed manually\n");
#endif

	printf("\n╔════════════════════════════════════════════════════════╗\n");
	printf("║  Example Completed Successfully!                       ║\n");
	printf("╚════════════════════════════════════════════════════════╝\n");

	return (0);
}
