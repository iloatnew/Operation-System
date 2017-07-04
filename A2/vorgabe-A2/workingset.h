#ifndef WORKINGSET_H
#define WORKINGSET_H

// Die zu verteilenden Aufgaben
struct WorkingSet {
    // Eingabe-Array dieses WorkingSets
    int *input;
    
    // Ausgabe-Array dieses WorkingSets
    int *output;
    
    // Laenge des WorkingSets
    int length;
    
    // Operation die auf den Daten ausgefuehrt werden soll
    int (*operation)(int, struct WorkingSet*);
    
    // Flag ob dieses WorkingSet schon bearbeitet wurde
    char done;
};

// Typdefinition fuer besseren Umgang
typedef struct WorkingSet WorkingSet;

#endif
