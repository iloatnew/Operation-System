#ifndef BS_AUFGABE_3_H
#define BS_AUFGABE_3_H

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>

#define THREAD_NUM 8
#define MAX_REQUEST_SIZE 256
#define MAX_REQUEST_NUM 4
#define AVAILABLE_MEMORY 2048

struct MemoryManagement {

	unsigned int available;	/* Der verfügbare Speicher */
	pthread_mutex_t lock;   /* Ein Mutex zum sichern des Speicherzugriffs */

};

extern struct MemoryManagement mm;

struct ThreadData {
	pthread_t thread_id; 					/* Die Thread Nummer */

	unsigned int requests[MAX_REQUEST_NUM]; /* Die Anfragen pro Zyklus */

	unsigned int used; 			/* Der Speicher, den dieser Thread verwendet */
	unsigned int requested; 		/* Der Speicher, auf den dieser Thread wartet */

	sem_t sem; 				/* Der "Wecker" dieses Threads */
};

/* Ein Feld von ThreadDaten */
extern struct ThreadData thread_data[THREAD_NUM];

/* Ob der Hauptthread weiter laufen soll */
extern int running;

/* vorgabe3.c - Initialisierungs- und Hilfsfunktionen */
void initialize_memory_management();
void print_thread_data();

/* Diese Funktionen sollt Ihr implementieren */

/* b) aufgabe3_semaphores.c */

void initialize_semaphores();
void destroy_semaphores();

/* c) aufgabe3_memory.c */

void request_memory(struct ThreadData *td, int amount);
void free_all_memory(struct ThreadData *td);

/* d) aufgabe3_threads.c */

void start_threads();
void cleanup();
void *thread_function(void *args);

/* e) aufgabe3_deadlock.c*/

void check_for_deadlocks();

/* Sternchen) aufgabe3_sanity.c*/

int check_save_allocate(int amount);

#endif
