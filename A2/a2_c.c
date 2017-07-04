#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include "aufgabe2.h"

#include "workingset.h"

WorkingSet tasks[TASK_NUM];

WorkingSet blur_tasks[TASK_NUM];

pthread_mutex_t lock;    //für global done
pthread_mutex_t pplock;  //für global done post process
// Wieviele Aufgaben wurden erledigt?
int global_done;

// Wieviel hat der jeweilige Thread erledigt?
int thread_done[THREAD_NUM];

// Die Eingabewerte der Berechnung
int input_values[ARRAY_SIZE];

// Das Ergebnis der Berechnung
int results[ARRAY_SIZE];

// Ergebnisse fuer Nachbearbeitung
int results_blur[ARRAY_SIZE];

// Wieviele Nachbearbeitungs-Aufgaben wurden erledigt?
int global_done_post_process;

// Die Thread-Funktion
void* run(void *args)
{ 
     
    int *thread_counter = (int*)args;		//thread_counter is a pointer, which points to args
    int res;
    while(global_done < TASK_NUM)
    {

        // Eine Aufgabe auswuerfeln
        int task_num = rand() % TASK_NUM;
        // Pointer auf das WorkingSet aufloesen
        
        WorkingSet *task = &tasks[task_num];	//ein zufalliger Task wählen
	res = pthread_mutex_lock(&task->lock);
        if(res!=0){
             perror("locken");
             exit(EXIT_FAILURE);
        }
        if(task->done==0){			//wenn er noch nicht bearbeitet wurde:
	     do_WorkingSet(task);	 	//die Funtion durchführen
	     res=pthread_mutex_lock(&lock);
             if(res!=0){
        	perror("locken");
                exit(EXIT_FAILURE);
             }
	     global_done++;                         //global_done erhören
	     res=pthread_mutex_unlock(&lock);	    //für global done 			
             if(res!=0){
             	perror("unlocken");
                exit(EXIT_FAILURE);
             }
	     (*thread_counter)++;		//Inkrementierung
             task->done = 1;
             res=pthread_mutex_unlock(&task->lock);
             if(res!=0){
			perror("unlocken");
			exit(EXIT_FAILURE);
	     }
             pthread_cond_signal(&task->taskdone);        //signal geben. da es schon sicher die Vorarbeit gemacht hat.	
	}
	else{
		res=pthread_mutex_unlock(&task->lock);
		if(res!=0){
			perror("unlocken");
			exit(EXIT_FAILURE);
		}
	}
	
    }
    pthread_exit(NULL);
}

void *post_process(void *args) {
    while(global_done_post_process < TASK_NUM)
    {
	int res;
        // Eine Aufgabe auswuerfeln
        int task_num = rand() % TASK_NUM;

        // Pointer auf das WorkingSet aufloesen
        WorkingSet *inputTask = &tasks[task_num];
        
        WorkingSet *outputTask = &blur_tasks[task_num];

	pthread_mutex_lock(&outputTask->lock);      // lock für diesen Task
        pthread_mutex_lock(&inputTask->lock);       //fragt, ob Vorarbeit gemacht wurde?
	if(inputTask->done == 0) 
	{	
		pthread_cond_wait(&inputTask->taskdone, &inputTask->lock);
	} 
	pthread_mutex_unlock(&inputTask->lock);
	//printf("ff\n");
	
	if(outputTask->done==0){		
	     do_WorkingSet(outputTask);	 	  // arbeitet post process
	     res=pthread_mutex_lock(&pplock);     // lock für diesen Task
             if(res!=0){
        	perror("locken");
                exit(EXIT_FAILURE);
             }
	     global_done_post_process++;           //für global done post process
              outputTask->done = 1;	
	     res=pthread_mutex_unlock(&pplock);	  //  Nacharbeit DONE
             if(res!=0){
             	perror("unlocken");
                exit(EXIT_FAILURE);
             }
             
	}
	res=pthread_mutex_unlock(&outputTask->lock);  //diesen Task wurde durchgeführt. freigeben
        if(res!=0){
             	perror("unlocken");
                exit(EXIT_FAILURE);
        }

    }
    pthread_exit(NULL);
}

void do_WorkingSet(WorkingSet *workingSet) {
	for(int i=0;i<workingSet->length;i++){
    	workingSet->output[i] = workingSet->operation(i, workingSet); 
        }
        workingSet->done = 1; 
}

/* Pseudozufallszahlengenerator initialisieren, Arrays befuellen */
int main(int argc, char **argv)
{
 // Initialisiere die Datenkorruption
    initialize();
    int res;
    res = pthread_mutex_init(&lock, NULL);
    if(res!=0){
        perror("lock initialisieren");
        exit(EXIT_FAILURE);
    }
    for(int i=0;i<THREAD_NUM;i++){
        res=pthread_mutex_init(&tasks[i].lock,NULL);
	//res=pthread_mutex_init(&tasks[i].lock,NULL);
        if(res!=0){
             perror("lock initialisieren");
             exit(EXIT_FAILURE);
        }
        res=pthread_mutex_init(&blur_tasks[i].lock,NULL);
        if(res!=0){
             perror("lock initialisieren");
             exit(EXIT_FAILURE);
        }
    }

    // Hier sollte euer Code hin
    pthread_t tid[THREAD_NUM];
    pthread_t tid2[THREAD_NUM];
    for(int i=0;i<THREAD_NUM;i++){
    	res=pthread_create(&tid[i],NULL,*run,&thread_done[i]);
        if(res!=0){
        perror("thread erstellen");
        exit(EXIT_FAILURE);
        }
        res=pthread_create(&tid2[i],NULL,*post_process,NULL);
        if(res!=0){
        perror("thread erstellen");
        exit(EXIT_FAILURE);
        }
    }
    for(int i=0;i<THREAD_NUM;i++){
	res=pthread_join(tid[i],NULL);
        if(res!=0){
        perror("thread join");
        exit(EXIT_FAILURE);
        }
        res=pthread_join(tid2[i],NULL);
        if(res!=0){
        perror("thread join");
        exit(EXIT_FAILURE);
        }
    } 
    for(int i=0;i<THREAD_NUM;i++){
        res=pthread_mutex_destroy(&tasks[i].lock);
        if(res!=0){
        perror("lock zerstören");
        exit(EXIT_FAILURE);
        }
        res=pthread_mutex_destroy(&blur_tasks[i].lock);
        if(res!=0){
        perror("lock zerstören");
        exit(EXIT_FAILURE);
        }
    }

	res=pthread_mutex_destroy(&lock);
        if(res!=0){
        perror("lock zerstören");
        exit(EXIT_FAILURE);
        }
    // Pruefe auf Datenkorruption
    test_results();
}

