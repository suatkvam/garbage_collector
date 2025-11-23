/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   examples_linked_list_example_Version2.c            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 22:29:43 by akivam              #+#    #+#             */
/*   Updated: 2025/11/23 22:29:43 by akivam             ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

/**
 * LINKED LIST EXAMPLE
 * 
 * Demonstrates how GC handles data structures automatically.
 * Shows the power of automatic memory management with linked lists.
 */

/* ************************************************************************** */
/*                      LINKED LIST STRUCTURE                                 */
/* ************************************************************************** */

typedef struct s_node
{
	int				value;
	char			*data;
	struct s_node	*next;
}	t_node;

/* ************************************************************************** */
/*                      LINKED LIST FUNCTIONS                                 */
/* ************************************************************************** */

t_node	*create_node(int value, const char *data)
{
	t_node	*node;
	int		i;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->value = value;
	
	// Allocate and copy data string
	i = 0;
	while (data[i])
		i++;
	node->data = malloc(i + 1);
	if (!node->data)
		return (NULL);
	i = 0;
	while (data[i])
	{
		node->data[i] = data[i];
		i++;
	}
	node->data[i] = '\0';
	
	node->next = NULL;
	return (node);
}

void	list_append(t_node **head, int value, const char *data)
{
	t_node	*new_node;
	t_node	*current;

	new_node = create_node(value, data);
	if (!new_node)
		return;
	
	if (!*head)
	{
		*head = new_node;
		return;
	}
	
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

void	print_list(t_node *head)
{
	t_node	*current;

	current = head;
	while (current)
	{
		printf("   [%d: %s]", current->value, current->data);
		if (current->next)
			printf(" -> ");
		current = current->next;
	}
	printf("\n");
}

int	list_length(t_node *head)
{
	int		count;
	t_node	*current;

	count = 0;
	current = head;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

void	free_list(t_node *head)
{
	t_node	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head->data);
		free(head);
		head = tmp;
	}
}

/* ************************************************************************** */
/*                              MAIN PROGRAM                                  */
/* ************************************************************************** */

int main(void)
{
	t_node	*list;
	t_node	*temp_list;

	printf("╔════════════════════════════════════════════════════════╗\n");
	printf("║     Linked List with GC Example                       ║\n");
	printf("╚════════════════════════════════════════════════════════╝\n\n");

#ifdef USE_GC_WRAP
	printf("Mode: GARBAGE COLLECTOR\n\n");
#else
	printf("Mode: STANDARD MALLOC/FREE\n\n");
#endif

	// ========== Example 1: Build List ==========
	printf("1. Building Linked List:\n");
	list = NULL;
	list_append(&list, 1, "First");
	list_append(&list, 2, "Second");
	list_append(&list, 3, "Third");
	list_append(&list, 4, "Fourth");
	list_append(&list, 5, "Fifth");
	
	printf("   Created list with %d nodes:\n", list_length(list));
	print_list(list);
	printf("\n");

	// ========== Example 2: Extend List ==========
	printf("2. Extending List:\n");
	list_append(&list, 6, "Sixth");
	list_append(&list, 7, "Seventh");
	list_append(&list, 8, "Eighth");
	
	printf("   Extended list to %d nodes:\n", list_length(list));
	print_list(list);
	printf("\n");

	// ========== Example 3: Temporary List ==========
	printf("3. Creating Temporary List:\n");
	temp_list = NULL;
	list_append(&temp_list, 100, "Temp1");
	list_append(&temp_list, 200, "Temp2");
	list_append(&temp_list, 300, "Temp3");
	
	printf("   Temporary list:\n");
	print_list(temp_list);
	
#ifdef USE_GC_WRAP
	printf("\n   Setting temp_list = NULL (becomes unreachable)\n");
	temp_list = NULL;
	printf("   GC will collect these nodes automatically!\n");
#else
	printf("\n   Freeing temporary list manually...\n");
	free_list(temp_list);
	printf("   Temporary list freed\n");
#endif
	printf("\n");

	// ========== Example 4: Truncate List ==========
	printf("4. Truncating Main List:\n");
	printf("   Original list (%d nodes):\n", list_length(list));
	print_list(list);
	
	printf("\n   Keeping only first 3 nodes...\n");
	if (list && list->next && list->next->next)
		list->next->next->next = NULL;
	
	printf("   Truncated list (%d nodes):\n", list_length(list));
	print_list(list);

#ifdef USE_GC_WRAP
	printf("\n   Nodes 4-8 are now unreachable\n");
	printf("   GC will collect them automatically!\n");
#endif
	printf("\n");

	// ========== Example 5: Cycle Test ==========
	printf("5. Handling Cycles:\n");
	t_node *cycle_list = create_node(1, "Node1");
	if (cycle_list)
	{
		cycle_list->next = create_node(2, "Node2");
		if (cycle_list->next)
		{
			cycle_list->next->next = create_node(3, "Node3");
			if (cycle_list->next->next)
			{
				// Create cycle: 3 -> 1
				cycle_list->next->next->next = cycle_list;
				printf("   Created cycle: 1 -> 2 -> 3 -> 1\n");
				
#ifdef USE_GC_WRAP
				printf("   Mark-and-sweep handles cycles correctly!\n");
				// Break cycle before losing reference
				cycle_list->next->next->next = NULL;
				cycle_list = NULL;
				printf("   Cycle broken and collected\n");
#else
				printf("   Breaking cycle and freeing manually...\n");
				cycle_list->next->next->next = NULL;
				free_list(cycle_list);
				printf("   Cycle handled\n");
#endif
			}
		}
	}
	printf("\n");

	// ========== Final Cleanup ==========
	printf("6. Final State:\n");
	printf("   Main list (%d nodes):\n", list_length(list));
	print_list(list);

#ifdef USE_GC_WRAP
	printf("\n   In GC mode:\n");
	printf("   ✓ All nodes are tracked\n");
	printf("   ✓ Unreachable nodes collected automatically\n");
	printf("   ✓ No manual free() needed\n");
	// Optional: call free_list anyway (becomes no-op)
	free_list(list);
#else
	printf("\n   In normal mode:\n");
	printf("   ✓ Must free all nodes manually\n");
	printf("   Freeing main list...\n");
	free_list(list);
	printf("   ✓ All memory freed\n");
#endif

	printf("\n╔════════════════════════════════════════════════════════╗\n");
	printf("║  Example Completed!                                    ║\n");
	printf("╚════════════════════════════════════════════════════════╝\n");

	return (0);
}
