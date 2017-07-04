#include "vorgabe3.h"

/***************************************************************/
/* In dieser Datei die Semaphoren initialisieren und zerstören */
/***************************************************************/

void initialize_semaphores()
{
	for(int i = 0; i < THREAD_NUM ; i++){
		if(sem_init(&(thread_data[i].sem), 0, 1)==-1){
			perror("Initialize semaphores");
			exit(EXIT_FAILURE);
		}
	}
}

void destroy_semaphores()
{
	for(int i = 0; i < THREAD_NUM ; i++){
		if(sem_destroy(&(thread_data[i].sem))==-1){
			perror("destroy semaphores");
			exit(EXIT_FAILURE);
		}
	}
}
