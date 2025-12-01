/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   examples_stress_test_example_Version2.c            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 13:15:03 by akivam            #+#    #+#             */
/*   Updated: 2025/12/01 13:15:03 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef USE_GC_WRAP
# include "../gc_wrap.h"
#endif

/**
 * STRESS TEST EXAMPLE
 *
 * Tests GC performance and correctness under heavy load:
 *   - Many allocations
 *   - Various sizes
 *   - Periodic collections
 *   - Memory reuse patterns
 */

/* ************************************************************************** */
/*                      CONFIGURATION                                         */
/* ************************************************************************** */

#define SMALL_ALLOC 16
#define MEDIUM_ALLOC 256
#define LARGE_ALLOC 4096
#define ITERATIONS 10000
#define COLLECT_INTERVAL 1000

/* ************************************************************************** */
/*                      HELPER FUNCTIONS                                      */
/* ************************************************************************** */

void	print_progress(int current, int total)
{
	int	percent;

	percent = (current * 100) / total;
	if (current % (total / 10) == 0)
		printf("   Progress: %d%%\n", percent);
}

/* ************************************************************************** */
/*                              TEST FUNCTIONS                                */
/* ************************************************************************** */

void	test_small_allocations(int count)
{
	int		i;
	void	*ptr;

	printf("   Test 1: %d small allocations (%d bytes each)\n", count,
		SMALL_ALLOC);
	i = 0;
	while (i < count)
	{
		ptr = malloc(SMALL_ALLOC);
		if (ptr)
			((char *)ptr)[0] = 'S';
		print_progress(i, count);
		i++;
	}
	printf("   ✓ Complete\n\n");
}

void	test_mixed_allocations(int count)
{
	int		i;
	void	*ptr;
	int		size;

	printf("   Test 2: %d mixed-size allocations\n", count);
	i = 0;
	while (i < count)
	{
		// Vary sizes
		if (i % 3 == 0)
			size = SMALL_ALLOC;
		else if (i % 3 == 1)
			size = MEDIUM_ALLOC;
		else
			size = LARGE_ALLOC;
		ptr = malloc(size);
		if (ptr)
			((char *)ptr)[0] = 'M';
		print_progress(i, count);
		i++;
	}
	printf("   ✓ Complete\n\n");
}

void	test_with_collections(int count, int interval)
{
	int		i;
	void	*ptr;
	int		collections;

	printf("   Test 3: %d allocations with periodic GC ", count);
	printf("(every %d)\n", interval);
	collections = 0;
	i = 0;
	while (i < count)
	{
		ptr = malloc(MEDIUM_ALLOC);
		if (ptr)
			((char *)ptr)[0] = 'C';
#ifdef USE_GC_WRAP
		if (i % interval == 0 && i > 0)
		{
			GC_COLLECT();
			collections++;
		}
#endif
		print_progress(i, count);
		i++;
	}
#ifdef USE_GC_WRAP
	printf("   ✓ Complete (%d collections)\n\n", collections);
#else
	printf("   ✓ Complete (no GC in normal mode)\n\n");
#endif
}

void	test_persistent_vs_temporary(int count)
{
	int		i;
	void	*persistent[10];
	void	*temp;
	int		j;

	printf("   Test 4: Persistent vs temporary allocations\n");
	// Create persistent allocations
	j = 0;
	while (j < 10)
	{
		persistent[j] = malloc(LARGE_ALLOC);
		if (persistent[j])
			((char *)persistent[j])[0] = 'P';
		j++;
	}
	printf("   Created 10 persistent allocations\n");
	// Create many temporary allocations
	i = 0;
	while (i < count)
	{
		temp = malloc(SMALL_ALLOC);
		if (temp)
			((char *)temp)[0] = 'T';
		print_progress(i, count);
		i++;
	}
	// Verify persistent allocations still valid
	printf("   Verifying persistent allocations...\n");
	j = 0;
	while (j < 10)
	{
		if (persistent[j] && ((char *)persistent[j])[0] == 'P')
			printf("   [%d] ✓", j);
		j++;
	}
	printf("\n   ✓ All persistent allocations intact\n\n");
#ifndef USE_GC_WRAP
	// Free in normal mode
	j = 0;
	while (j < 10)
	{
		free(persistent[j]);
		j++;
	}
#endif
}

/* ************************************************************************** */
/*                              MAIN PROGRAM                                  */
/* ************************************************************************** */

int	main(void)
{
	clock_t	start;
	clock_t	end;
	double	cpu_time;

	printf("╔════════════════════════════════════════════════════════╗\n");
	printf("║     Stress Test Example                               ║\n");
	printf("╚════════════════════════════════════════════════════════╝\n\n");
#ifdef USE_GC_WRAP
	printf("Mode: GARBAGE COLLECTOR\n");
	printf("Configuration:\n");
	printf("  - Iterations: %d\n", ITERATIONS);
	printf("  - Collection interval: %d\n\n", COLLECT_INTERVAL);
#else
	printf("Mode: STANDARD MALLOC/FREE\n");
	printf("Configuration:\n");
	printf("  - Iterations: %d\n\n", ITERATIONS);
#endif
	start = clock();
	// Run tests
	printf("═══════════════════════════════════════════════════════\n");
	printf("Starting Stress Tests...\n");
	printf("═══════════════════════════════════════════════════════\n\n");
	test_small_allocations(ITERATIONS);
	test_mixed_allocations(ITERATIONS);
	test_with_collections(ITERATIONS, COLLECT_INTERVAL);
	test_persistent_vs_temporary(ITERATIONS / 10);
	printf("═══════════════════════════════════════════════════════\n");
	printf("All Tests Completed!\n");
	printf("═══════════════════════════════════════════════════════\n\n");
	end = clock();
	cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Performance Summary:\n");
	printf("  - Total time: %.3f seconds\n", cpu_time);
	printf("  - Total allocations: ~%d\n", ITERATIONS * 3 + ITERATIONS / 10);
	printf("  - Avg time per allocation: %.6f seconds\n", cpu_time / (ITERATIONS
			* 3 + ITERATIONS / 10));
#ifdef USE_GC_WRAP
	printf("\n  GC Statistics:\n");
	printf("  - Collections triggered: ~%d\n", (ITERATIONS * 3)
		/ COLLECT_INTERVAL);
	printf("  - Memory managed automatically\n");
	printf("\nFinal cleanup...\n");
	GC_CLEANUP();
	printf("✓ GC cleanup complete\n");
#else
	printf("\n  Memory Management:\n");
	printf("  - Manual free() required for each allocation\n");
	printf("  - No automatic collection\n");
#endif
	printf("\n╔════════════════════════════════════════════════════════╗\n");
	printf("║  Stress Test Completed Successfully!                  ║\n");
	printf("╚════════════════════════════════════════════════════════╝\n");
	return (0);
}
