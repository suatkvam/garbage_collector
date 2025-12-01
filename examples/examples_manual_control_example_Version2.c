/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   examples_manual_control_example_Version2.          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 13:15:03 by akivam            #+#    #+#             */
/*   Updated: 2025/12/01 13:15:03 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#ifdef USE_GC_WRAP
# include "../gc_wrap.h"
#endif

/**
 * MANUAL CONTROL EXAMPLE
 *
 * This example shows optional manual control of GC.
 * You can:
 *   - Manually initialize GC (optional, auto-init works)
 *   - Manually trigger collections (optional)
 *   - Manually cleanup (optional, happens at exit)
 *
 * Note: Manual control is completely optional!
 */

/* ************************************************************************** */
/*                      HELPER FUNCTIONS                                      */
/* ************************************************************************** */

void	allocate_batch(int count, size_t size)
{
	int		i;
	void	*ptr;

	i = 0;
	while (i < count)
	{
		ptr = malloc(size);
		if (ptr)
		{
			// Use memory...
			((char *)ptr)[0] = 'X';
		}
		i++;
	}
}

void	print_separator(void)
{
	printf("────────────────────────────────────────────────────────\n");
}

/* ************************************************************************** */
/*                              MAIN PROGRAM                                  */
/* ************************************************************************** */

int	main(void)
{
	int		stack_var;
	char	*persistent;
	int		i;

	printf("╔════════════════════════════════════════════════════════╗\n");
	printf("║     Manual Control Example (Optional)                 ║\n");
	printf("╚════════════════════════════════════════════════════════╝\n\n");
#ifdef USE_GC_WRAP
	printf("Mode: GARBAGE COLLECTOR\n\n");
	// ========== Optional: Manual Initialization ==========
	printf("1. Manual Initialization (optional):\n");
	printf("   Calling GC_INIT()...\n");
	GC_INIT(stack_var);
	printf("   ✓ GC initialized manually\n");
	printf("   (Note: GC auto-initializes on first malloc if not called)\n");
	print_separator();
	printf("\n");
#else
	printf("Mode: STANDARD MALLOC/FREE\n\n");
	printf("Manual control features not available in normal mode.\n");
	printf("Running standard operations...\n\n");
#endif
	// ========== Example 1: Persistent Allocation ==========
	printf("2. Creating Persistent Allocation:\n");
	persistent = malloc(100);
	if (persistent)
	{
		for (i = 0; i < 10; i++)
			persistent[i] = 'A' + i;
		persistent[10] = '\0';
		printf("   Allocated persistent data: %s...\n", persistent);
	}
	print_separator();
	printf("\n");
	// ========== Example 2: Batch Allocations ==========
	printf("3. Batch Allocations (Phase 1):\n");
	printf("   Allocating 50 blocks of 1KB each...\n");
	allocate_batch(50, 1024);
	printf("   ✓ Phase 1 complete\n");
	print_separator();
	printf("\n");
#ifdef USE_GC_WRAP
	// ========== Example 3: Manual Collection ==========
	printf("4. Manual Collection (optional):\n");
	printf("   Calling GC_COLLECT()...\n");
	GC_COLLECT();
	printf("   ✓ Collection completed\n");
	printf("   Persistent data still valid: %s...\n", persistent);
	print_separator();
	printf("\n");
#endif
	// ========== Example 4: More Allocations ==========
	printf("5. Batch Allocations (Phase 2):\n");
	printf("   Allocating 100 blocks of 512B each...\n");
	allocate_batch(100, 512);
	printf("   ✓ Phase 2 complete\n");
	print_separator();
	printf("\n");
	// ========== Example 5: Periodic Collection Pattern ==========
	printf("6. Periodic Collection Pattern:\n");
	printf("   Processing data in batches with periodic collection...\n");
	i = 0;
	while (i < 10)
	{
		printf("   Batch %d: ", i + 1);
		allocate_batch(20, 256);
		printf("allocated 20 blocks");
#ifdef USE_GC_WRAP
		if (i % 3 == 0)
		{
			printf(", collecting...");
			GC_COLLECT();
		}
#endif
		printf("\n");
		i++;
	}
	printf("   ✓ All batches processed\n");
	print_separator();
	printf("\n");
	// ========== Example 6: Final State ==========
	printf("7. Final State:\n");
	printf("   Persistent data: %s...\n", persistent);
#ifdef USE_GC_WRAP
	printf("   All other allocations can be collected\n");
	print_separator();
	printf("\n");
	// ========== Optional: Manual Cleanup ==========
	printf("8. Manual Cleanup (optional):\n");
	printf("   Calling GC_CLEANUP()...\n");
	GC_CLEANUP();
	printf("   ✓ GC cleaned up\n");
	printf("   (Note: GC auto-cleans at program exit if not called)\n");
#else
	// In normal mode, free manually
	free(persistent);
	printf("   Freed persistent data manually\n");
#endif
	printf("\n╔════════════════════════════════════════════════════════╗\n");
	printf("║  Example Completed!                                    ║\n");
	printf("╚════════════════════════════════════════════════════════╝\n");
	return (0);
}
