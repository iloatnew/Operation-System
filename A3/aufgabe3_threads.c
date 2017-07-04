#include "vorgabe3.h"

#include <unistd.h>

/**********************************************************/
/* In dieser Datei die Threads implementieren und starten */
/**********************************************************/

struct ThreadData thread_data[THREAD_NUM];

// Anfragen "request_memory" und Warten(1s). Wenn alle Anfragen beantwortet wurden, ruft "free_all_memory"
void* thread_function(void *args)
{
	struct ThreadData *this = (struct ThreadData *) args;
	// Wartesemaphore setzen
	if(sem_wait(&this->sem)==-1){
			perror("sem_wait");
			exit(1);
	}
	//Endlosschleife während das Program läuft
	while(running){
		//Frage immer nach Speicher und lege dich dann schlafen
		for(int j = 0; j < MAX_REQUEST_NUM ; j++){
			this->requested = this->requests[j];
			request_memory(this, this->requests[j]);
			sleep(1);
		}
		//Gib deinen Speicher wieder frei
		printf("Thread %ld wurde erfolgreich bearbeitet, starte Thread neu...\n", this->thread_id);
		free_all_memory(this);
		
	}	
	pthread_exit(NULL);
}

void cleanup()
{
	//cleanup wird hier zwar nur in Deadlock aufgerufen, soll aber nur "aufräumen" wen running auch auf 0 gesetzt ist
	if (running != 0) return;

	//Da die Threads in einem Deadlock sind, müssen wir sie mit pthread_cancel zwanghaft beenden.
	for(int i = 0; i < THREAD_NUM ; i++){
		if( pthread_cancel( (thread_data[i].thread_id) )!=0){
			perror("pthread_cancel");
			exit(EXIT_FAILURE);
		}
	}

	if(pthread_mutex_destroy(&mm.lock) ) {
		perror("pthread_mutex_destroy");
		exit(EXIT_FAILURE);
	}
}

void start_threads()
{
	//thread_num = 8
	for(int i = 0; i < THREAD_NUM ; i++)
	{
		// Max_Request_Num = 4
		for(int j = 0; j < MAX_REQUEST_NUM ; j++)
		{
			//Die Anfragen pro Zyklus for this thread. between 1 and 256
			thread_data[i].requests[j] = (rand() % MAX_REQUEST_SIZE) + 1;
			// So that the max requested memory is 4*245 = 1024 in one thread
		}
		if(pthread_create(&(thread_data[i].thread_id), NULL, &thread_function, thread_data + i)) {
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	} 
}
