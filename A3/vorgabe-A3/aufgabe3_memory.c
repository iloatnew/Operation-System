#include "vorgabe3.h"

/*********************************************/
/* Implementiert hier die Speicherverwaltung */
/*********************************************/

void request_memory(struct ThreadData *td, int amount)
{

}

void free_all_memory(struct ThreadData *td)
{
	if (pthread_mutex_lock(&(mm.lock))) {
		perror("pthread_mutex_lock");
		exit(1);
	}
	/* Allen Speicher freigeben */
	mm.available += td->used;
	td->used = 0;
	/* Erst besten Faden wieder aufwecken */
	for(int i = 0; i < THREAD_NUM; i++)
	{
		if(thread_data[i].requested != 0 && thread_data[i].requested <= mm.available)
		{
		/* Angefragten Speicher nachträglich reservieren */
			mm.available -= thread_data[i].requested;
			thread_data[i].used += thread_data[i].requested;
			thread_data[i].requested = 0;
			if (sem_post(&(thread_data[i].sem))) {
				perror("sem_post");
				exit(1);
			}
		}
	} 
	if (pthread_mutex_unlock(&(mm.lock))) {
		perror("pthread_mutex_unlock");
		exit(1);
	}
}
