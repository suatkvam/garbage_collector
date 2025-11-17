#include "internal_collecter.h"

void	collecter_close(void)
{
	t_collecter **head_ptr;
	t_collecter *current;
	t_collecter *next;

	head_ptr = get_gc_head();
	current = *head_ptr;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	*head_ptr = NULL;
}