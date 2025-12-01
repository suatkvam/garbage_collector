/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_wrap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 22:29:43 by akivam              #+#    #+#             */
/*   Updated: 2025/11/28 20:25:14 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

/**
 * WRAPPER SYSTEM - NO GLOBAL VARIABLES
 * 
 * Uses static functions to maintain state without globals
 */

/* ************************************************************************** */
/*                      REAL FUNCTIONS (from libc)                            */
/* ************************************************************************** */

void	*__real_malloc(size_t size);
void	*__real_calloc(size_t nmemb, size_t size);
void	*__real_realloc(void *ptr, size_t size);
void	__real_free(void *ptr);

/* ************************************************************************** */
/*                      STATE MANAGEMENT (no globals!)                        */
/* ************************************************************************** */

static int	get_initialized_state(int set, int value)
{
	static int	initialized = 0;

	if (set)
		initialized = value;
	return (initialized);
}

static int	is_initialized(void)
{
	return (get_initialized_state(0, 0));
}

static void	set_initialized(int value)
{
	get_initialized_state(1, value);
}

/**
 * auto_init_gc - Automatically initialize GC on first malloc
 * 
 * CRITICAL FIX: stack_anchor must be static to persist after function returns
 * Otherwise, stack_start pointer would be invalid!
 */
static void	auto_init_gc(void)
{
	static int	stack_anchor;

	if (!is_initialized())
	{
		collector_init(&stack_anchor);
		set_initialized(1);
	}
}

/* ************************************************************************** */
/*                      WRAPPED FUNCTIONS                                     */
/* ************************************************************************** */

void	*__wrap_malloc(size_t size)
{
#ifdef USE_GC_WRAP
	auto_init_gc();
	return (gc_malloc(size));
#else
	return (__real_malloc(size));
#endif
}

void	*__wrap_calloc(size_t nmemb, size_t size)
{
#ifdef USE_GC_WRAP
	auto_init_gc();
	return (gc_calloc(nmemb, size));
#else
	return (__real_calloc(nmemb, size));
#endif
}

void	*__wrap_realloc(void *ptr, size_t size)
{
#ifdef USE_GC_WRAP
	auto_init_gc();
	return (gc_realloc(ptr, size));
#else
	return (__real_realloc(ptr, size));
#endif
}

void	__wrap_free(void *ptr)
{
#ifdef USE_GC_WRAP
	(void)ptr;
#else
	__real_free(ptr);
#endif
}

/* ************************************************************************** */
/*                      OPTIONAL MANUAL CONTROL                               */
/* ************************************************************************** */

void	gc_init_manual(void *stack_start)
{
	if (!is_initialized())
	{
		collector_init(stack_start);
		set_initialized(1);
	}
}

void	gc_collect_manual(void)
{
	if (is_initialized())
		gc_collect();
}

void	gc_cleanup_manual(void)
{
	if (is_initialized())
	{
		collector_close();
		set_initialized(0);
	}
