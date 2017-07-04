#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "aufgabe2.h"
#include "workingset.h"


/* Die auszuführende Operation */
int increase_brightness(int index, WorkingSet *set)
{
    int tmpValue = *(set->input + index);
    return tmpValue * 2;
}

void initialize() {
    // Initialisieren der Zufallszahlen
    srand(SEED);
    
    // Zuruecksetzen der globalen Abschlussvariablen
    global_done = 0;
    
    // Tasks vorbereiten
    for(int i = 0; i < TASK_NUM; i++) {
        tasks[i].input = &input_values[i * (ARRAY_SIZE / TASK_NUM)];
        tasks[i].output = &results[i * (ARRAY_SIZE / TASK_NUM)];
        tasks[i].length = (ARRAY_SIZE / TASK_NUM);
        tasks[i].operation = &increase_brightness;
        tasks[i].done = 0;
    }
    
    // Befuellen der Datenarrays
    for(int i = 0; i < ARRAY_SIZE; i++) {
        input_values[i] = i;
    }
}

void test_results() {
    // Hier wird die Summe der abgeschlossenen Tasks berechnet
    int sum = 0;
	for(int i = 0; i < THREAD_NUM; i++) {
		printf("Thread %d hat %d Working-Sets bearbeitet\n", i, thread_done[i]);
        sum += thread_done[i];
    }
    printf("In der Summe wurden %d Tasks bearbeitet.\n", sum);
    if ( sum > TASK_NUM) {
        printf("Es wurden mehr Tasks bearbeitet als existieren!\n");
    }
    
    // Auf Korruption ueberpruefen
    check_for_corruption();
}

/**
 * Vorgegebene Funktion zur Ueberpruefung, ob eine Korruption der Daten
 * aufgetreten ist.
 * 
 */
void check_for_corruption()
{
    for(int i = 0; i < TASK_NUM; i++) {
        
        for(int elementIndex = 0; elementIndex < tasks[i].length; elementIndex++) {
            int compareValue = tasks[i].operation(elementIndex, &tasks[i]);
            if ( compareValue != *(tasks[i].output + elementIndex)) {
                printf("Korruption erkannt bei tasks[%d]_%d\n", i,elementIndex);
                printf("Soll-Wert: %d, Ist-Wert: %d\n",compareValue,*(tasks[i].output + elementIndex));
                exit(EXIT_FAILURE);
            }
        }
    }
    printf("Hauptaufgabe ohne Korruption bearbeitet, prima!\n");
}
