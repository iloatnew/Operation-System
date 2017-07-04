#include "vorgabe3.h"

/*********************************************/
/* Implementiert hier die Speicherverwaltung */
/*********************************************/

void request_memory(struct ThreadData *td, int amount)
{
	if (pthread_mutex_lock(&(mm.lock))) {
		perror("pthread_mutex_lock");
		exit(1);
	}
	td->requested = amount;
	// Berechnet die Speichermenge die für die komplette Bearbeitung noch benötigt wird
	int max_amount = 0;
	for( int i = 0; i< MAX_REQUEST_NUM; i++){
		max_amount += td->requests[i];
	}
	max_amount -= td->used;
	//
	// if more than available was requested, wait:
								//while(td->requested > mm.available){
	while(!check_save_allocate(max_amount)){
		// done, unlock
		if (pthread_mutex_unlock(&(mm.lock))) {
			perror("pthread_mutex_unlock");
			exit(1);
		}
		//Mutex freigegeben, leg dich schlafen
		if(sem_wait(&td->sem)==-1){
			perror("sem_wait");
			exit(1);
		}
		
		//lock again
		if (pthread_mutex_lock(&(mm.lock))) {
			perror("pthread_mutex_lock");
			exit(1);
		}

	}
	

	
	// Allokiere den Speicherplatz und setz deine Werte entsprechend
	mm.available -= td->requested;
	td->used += td->requested;
	td->requested = 0;
	
	
	// done, unlock
	if (pthread_mutex_unlock(&(mm.lock))) {
		perror("pthread_mutex_unlock");
		exit(1);
	}
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
		//Berechne wieviel Speicher maximal verbraucht wird und schaue, ob der restliche benötigte Speicher genug wäre.
		int max_amount = 0;
		for( int j = 0; j < MAX_REQUEST_NUM; j++){
			max_amount += thread_data[i].requests[j];
		}
		max_amount -= thread_data[i].used;
		// if any thread is requesting memories and the request can be fullfilled (due to enough amount of availabe memory) do:
		if(thread_data[i].requested != 0 && check_save_allocate(max_amount))
		{
		/* Angefragten Speicher nachträglich reservieren */
			//mm.available -= thread_data[i].requested;
			//thread_data[i].used += thread_data[i].requested;
			//thread_data[i].requested = 0;
			//V(this.sem); thread[i].sem = thread[i].sem + 1 

			//Wenn genug Speicherplatz verfügbar wäre, wecke den Thread auf (erneute Überprüfung findet in request memory statt)
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
