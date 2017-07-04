#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <assert.h>

#include "4a.h"

extern char bst[NUM_NODES];

void free_in_child(void *addr)
{
	pid_t pid = fork();
	int wstatus;

	if (pid == -1) {
		perror("fork");
		exit(1);
	}
	if (pid == 0) {
		buddy_free(addr);
		memory_dump();
		exit(0);
	}
	if (wait(&wstatus) == -1) {
		perror("wait");
		exit(1);
	}
	if (!WIFEXITED(wstatus)) {
		puts("FEHLER: Das Programm ist unsauber abgestuerzt");
		exit(1);
	}
	if (WEXITSTATUS(wstatus) == 0) {
		puts("FEHLER: Das Programm hat sich beendet, ohne einen Fehler zu signalisieren");
		exit(1);
	}
}

int main(void)
{
	/* Initialisierung des Binaerbaums fuer die Speicherverwaltung */
	bst[0] = NODE_SPLIT;
	bst[1] = 'A';
	bst[2] = NODE_SPLIT;
	bst[5] = NODE_SPLIT;
	bst[11] = 'B';
	bst[12] = NODE_SPLIT;
	bst[25] = NODE_SPLIT;
	bst[51] = 'C';
	bst[52] = NODE_SPLIT;
	bst[105] = 'D';

	puts("Ausgangssituation:");
	memory_dump();

	puts("--- Teste normale Operationen");

	puts("buddy_free(&A):");
	buddy_free(node_start_addr(&bst[1]));
	memory_dump();
	assert(bst[1] == NODE_FREE);
	assert(bst[0] == NODE_SPLIT);

	puts("buddy_free(&B):");
	buddy_free(node_start_addr(&bst[11]));
	memory_dump();
	assert(bst[11] == NODE_FREE);

	puts("buddy_free(&C):");
	buddy_free(node_start_addr(&bst[51]));
	memory_dump();
	assert(bst[51] == NODE_FREE);

	puts("buddy_free(&D):");
	buddy_free(node_start_addr(&bst[105]));
	memory_dump();
	assert(bst[105] == NODE_FREE);
	assert(bst[52] == NODE_FREE);
	assert(bst[25] == NODE_FREE);
	assert(bst[12] == NODE_FREE);
	assert(bst[5] == NODE_FREE);
	assert(bst[2] == NODE_FREE);
	assert(bst[0] == NODE_FREE);

	puts("--- Teste weiche Fehler");

	puts("buddy_free(NULL):");
	buddy_free(NULL);
	memory_dump();

	puts("--- Teste harte Fehler");

	puts("buddy_free(&D):");
	free_in_child(node_start_addr(&bst[120]));

	puts("--- Tests erfolgreich beendet");

	return 0;
}
