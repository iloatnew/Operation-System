#include "vorgabe3.h"

#include <unistd.h>

int running = 1;

int main(int argc, char **argv)
{
	srand(time(NULL));
	initialize_semaphores();
	initialize_memory_management();
	start_threads();
	
	while(running) {
		sleep(2);
		check_for_deadlocks();	
	}

	destroy_semaphores();
}
