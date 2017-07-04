#ifndef AUFGABE_2_H
#define AUFGABE_2_H
#include <stdint.h>
#include "workingset.h"

// Die Anzahl der zu parallel startenden Threads
#define THREAD_NUM 32

// Die Anzahl der WorkingSets, mit denen das Array bearbeitet werden soll.
#define TASK_NUM (THREAD_NUM * 64)

// Die Groesse des Datenbestandes
#define ARRAY_SIZE (TASK_NUM * 1024)

#define SEED 0xC0FFEE42

// Deklaration fuer globale Daten
extern WorkingSet tasks[TASK_NUM];

// Deklaration fuer globale Daten
extern WorkingSet blur_tasks[TASK_NUM];

// Die zu kombinierenden Werte
extern int input_values[ARRAY_SIZE];

// Die Ergebnisse der ersten Berechnung
extern int results[ARRAY_SIZE];

// Die Ergebnisse der Nachbearbeitung
extern int results_blur[ARRAY_SIZE];

// Wieviele Aufgaben wurden erledigt?
extern int global_done;

// Wieviel hat der jeweilige Thread erledigt?
extern int thread_done[THREAD_NUM];

// Funktionsdeklarationen
void initialize();
void test_results();

// Die Funktionen zur Berechnung des Mittelwerts und der Glaettung
int calculate_mean_value(int a, int b);
int unsharpen_values(int left_neighbor, int middle_value, int right_neighbor);

// Funktion zur Ueberpruefung auf Korruption
void check_for_corruption();

// Funktion zur Ueberpruefung auf Korruption
void check_for_corruption_blur();

// Funktion zur Bearbeitung eines WorkingSets
void do_WorkingSet(WorkingSet *workingSet);

#endif
