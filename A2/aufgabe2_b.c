#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include "aufgabe2.h"

#include "workingset.h"

WorkingSet tasks[TASK_NUM];

// Wieviele Aufgaben wurden erledigt?
int global_done;

// Wieviel hat der jeweilige Thread erledigt?
int thread_done[THREAD_NUM];

// Die Eingabewerte der Berechnung
int input_values[ARRAY_SIZE];

// Das Ergebnis der Berechnung
int results[ARRAY_SIZE];

pthread_mutex_t lock;

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
	res = pthread_mutex_lock(&task->lock);  //lock diesen WorkingSet
        if(res!=0){
             perror("locken");
             exit(EXIT_FAILURE);
        }
        if(task->done==0){			//wenn er noch nicht bearbeitet wurde:
	     do_WorkingSet(task);	 	//die Funtion durchführen
	     res=pthread_mutex_lock(&lock);     //global lock für das Parameter global_done
             if(res!=0){
        	perror("locken");
                exit(EXIT_FAILURE);
             }
	     global_done++;                             //global_done erhören
	     res=pthread_mutex_unlock(&lock);		  //global unlock	
             if(res!=0){
             	perror("unlocken");
                exit(EXIT_FAILURE);
             }
	     (*thread_counter)++;		//Inkrementierung
             task->done = 1;	
	}
	res=pthread_mutex_unlock(&task->lock);  //diesen Task WorkingSet
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
    // global lock
    res = pthread_mutex_init(&lock, NULL);
    if(res!=0){
        perror("lock initialisieren");
        exit(EXIT_FAILURE);
    }
    // Jeder Thread besitzt einen eingenen Lock
    for(int i=0;i<THREAD_NUM;i++){
        res=pthread_mutex_init(&tasks[i].lock,NULL);
        if(res!=0){
        perror("lock initialisieren");
        exit(EXIT_FAILURE);
        }
    }

    // Hier sollte euer Code hin
    pthread_t tid[THREAD_NUM];
    for(int i=0;i<THREAD_NUM;i++){
    	res=pthread_create(&tid[i],NULL,*run,&thread_done[i]);
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
    } 
    for(int i=0;i<THREAD_NUM;i++){
        res=pthread_mutex_destroy(&tasks[i].lock);
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
    
 
    return 0;
}

