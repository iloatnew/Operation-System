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

int blur_filter(int index, WorkingSet *set) {
    int left_neighbor = 0;
    int right_neighbor = 0;
    int middle_value = 0;
    if (index > 0 && (index < set->length - 1) ) {
        left_neighbor = *(set->input + (index-1));
        middle_value = *(set->input + (index));
        right_neighbor= *(set->input + (index + 1));
    } else {
        middle_value = *(set->input + (index));
    }

    return ((left_neighbor + (3*middle_value) + right_neighbor)/5);
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
    
    // Tasks vorbereiten
    for(int i = 0; i < TASK_NUM; i++) {
        blur_tasks[i].input = &results[i * (ARRAY_SIZE / TASK_NUM)];
        blur_tasks[i].output = &results_blur[i * (ARRAY_SIZE / TASK_NUM)];
        blur_tasks[i].length = (ARRAY_SIZE / TASK_NUM);
        blur_tasks[i].operation = &blur_filter;
        blur_tasks[i].done = 0;
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
    
    check_for_corruption_blur();
}

/**
 * Vorgegebene Funktion zur Ueberpruefung, ob eine Korruption der Daten
 * aufgetreten ist.
 * 
 * Im Falle einer Korruption gibt die Funktion den Wert 1 zurueck. Ansonsten 0.
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

void check_for_corruption_blur()
{
    for(int i = 0; i < TASK_NUM; i++) {
        
        for(int elementIndex = 0; elementIndex < blur_tasks[i].length; elementIndex++) {
            int compareValue = blur_tasks[i].operation(elementIndex, &blur_tasks[i]);
            if ( compareValue != *(blur_tasks[i].output + elementIndex)) {
                printf("Korruption erkannt bei Bonusaufgaben-Tasks[%d]_%d\n", i,elementIndex);
                printf("Soll-Wert: %d, Ist-Wert: %d\n",compareValue,*(blur_tasks[i].output + elementIndex));
                exit(EXIT_FAILURE);
            }
        }
    }
    printf("Bonusaufgabe ohne Korruption bearbeitet, prima!\n");
}
