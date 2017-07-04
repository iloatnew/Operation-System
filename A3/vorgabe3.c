#include "vorgabe3.h"

#include <stdlib.h>

struct MemoryManagement mm;

void initialize_memory_management()
{
	mm.available = AVAILABLE_MEMORY;
	pthread_mutex_init(&(mm.lock), NULL);
}

void print_thread_data()
{
	int sum = 0;
	for(int i = 0; i < THREAD_NUM; i++)
	{
		int needed = 0;
		for(int j = 0; j < MAX_REQUEST_NUM ; j++)
		{
			needed += thread_data[i].requests[j];
		}
		printf("Thread %d uses %dkB memory and is waiting for %dkB. A whole cylce needs %dkB memory.\n", i, thread_data[i].used, thread_data[i].requested, needed);
	sum += needed;
	}
	printf("All threads need at most %dkB memory.\n", sum);
	
}
