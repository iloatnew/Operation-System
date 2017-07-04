#include "vorgabe3.h"

#include <unistd.h>
/********************************************/
/* Implementiert hier die Deadlockerkennung */
/********************************************/

void check_for_deadlocks()
{
	//printf("checking dead!\n");

	int dead = 1;
	if (pthread_mutex_lock(&(mm.lock))) {
		perror("pthread_mutex_lock");
		exit(1);
	}
	/* Zum Debuggen */
	for(int i = 0; i < THREAD_NUM; i++){
		int max_amount = 0;
		for( int j = 0; j< MAX_REQUEST_NUM; j++){
			max_amount += thread_data[i].requests[j];
		}
		max_amount -= thread_data[i].used;
		//if(thread_data[i].requested<mm.available){     
		if(check_save_allocate(max_amount)){		//für bonus
			dead = 0;
			break;
		}
	}
	if (pthread_mutex_unlock(&(mm.lock))) {
		perror("pthread_mutex_unlock");
		exit(1);
	}
	if(dead){
		print_thread_data();
		running = 0;
		cleanup();
	}
		
}
