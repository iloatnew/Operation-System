#include <stdio.h>
#include <string.h>
#include <err.h>

#include "bst.h"

// Der Hauptspeicher (ist hier 4MB groß)
char heap[CHUNK_SIZE * NUM_CHUNKS];

// Der Binärbaum zur Speicherverwaltung (ist hier 127 Byte groß)
char bst[NUM_NODES];

// Hilfsfunktion, entspricht pow(2, x) aus math.h für Ganzzahlen
static inline unsigned int int_pow2(unsigned int x)
{
	return 1 << x;
}

// Hilfsfunktion, entspricht floor(log2(x)) aus math.h für Ganzzahlen
static inline unsigned int int_log2(unsigned int x)
{
	/* Sehr effizient, aber weniger portabel: */
	// return 31 - __builtin_clz(x);

	/* von uns genutzte portable Methode: */
	unsigned int log = 0;
	while (x >>= 1)
		log++;
	return log;
}

node_t bst_root(void)
{
	return bst;
}

node_t bst_left(node_t node)
{
	unsigned int dest_idx = 2 * (node - bst) + 1;
	if (dest_idx >= sizeof(bst)) {
		// node ist ein Blatt und hat keine Kinder mehr
		return NULL;
	}
	return &bst[dest_idx];
}

node_t bst_right(node_t node)
{
	unsigned int dest_idx = 2 * (node - bst) + 2;
	if (dest_idx > sizeof(bst)) {
		// node ist ein Blatt und hat keine Kinder mehr
		return NULL;
	}
	return &bst[dest_idx];
}

node_t bst_parent(node_t node)
{
	// Die Wurzel hat keinen Elternknoten
	if (node == bst)
		return NULL;

	return &bst[(node - bst - 1)/2];
}

void bst_housekeeping(node_t node)
{
	node_t left = bst_left(node);
	node_t right = bst_right(node);

	// Abbruchbedingung der Rekursion
	if (left == NULL)
		return;

	// Zuerst die Teilbäume der Kinder aufräumen...
	bst_housekeeping(left);
	bst_housekeeping(right);

	/*
	 * ... und dann den aktuellen Knoten. Falls beide Kinder komplett frei sind,
	 * der Knoten aber von "Kinder teilweise belegt" ausgeht, ist diese
	 * Information veraltet und der Knoten bzw. der zugehörige
	 * Speicherbereich auch frei.
	 */
	if ((*left == NODE_FREE) && (*right == NODE_FREE) && (*node == NODE_SPLIT))
		*node = NODE_FREE;
}

char get_node_content(node_t node)
{
	if (node == NULL)
		errx(1, "get_node_content(node) wurde mit node == NULL aufgerufen");
	return *node;
}

void set_node_content(node_t node, char value)
{
	if (node == NULL)
		errx(1, "set_node_content(node, value) wurde mit node == NULL aufgerufen");
	*node = value;
}

unsigned int node_level(node_t node)
{
	return BUDDY_LEVEL - int_log2(node - bst + 1);
}

unsigned int node_position(node_t node)
{
	return node - &bst[int_pow2(int_log2(node - bst + 1)) - 1];
}

unsigned int node_width_chunks(node_t node)
{
	return int_pow2(node_level(node));
}

unsigned int node_width_bytes(node_t node)
{
	return node_width_chunks(node) * CHUNK_SIZE;
}

unsigned int size_to_chunks(unsigned int bytes)
{
	if ((bytes % CHUNK_SIZE) != 0)
		bytes += (CHUNK_SIZE - bytes);

	unsigned int prev_power_of_two = int_pow2(int_log2(bytes / CHUNK_SIZE));

	if ((prev_power_of_two * CHUNK_SIZE) == bytes) {
		// Die Blockmenge ist bereits eine Zweierpotenz
		return prev_power_of_two;
	}

	// Die Blockmenge ist keine Zweierpotenz und muss aufgerundet werden
	return int_pow2(int_log2(bytes / CHUNK_SIZE) + 1);
}

void *node_start_addr(node_t node)
{
	return &heap[node_position(node) * node_width_bytes(node)];
}

void *node_split_addr(node_t node)
{
	if (bst_right(node) != NULL)
		return node_start_addr(bst_right(node));
	return node_start_addr(node);
}

void *node_end_addr(node_t node)
{
	return &heap[(node_position(node) + 1) * node_width_bytes(node) - 1];
}

void bst_dump(void)
{
	for (int i = 0; i < NUM_NODES; i++) {
		if (node_position(&bst[i]) == 0)
			putchar('\n');
		if (bst[i] <= NODE_FREE)
			printf("%3d", bst[i]);
		else
			printf("%3c", bst[i]);
	}
	putchar('\n');
}

void memory_dump_recursive(node_t node)
{
	if (node == bst) {
		puts("|      0M      ||      1M      ||      2M      ||      3M      |");
	}
	if (bst_left(node) == NULL) {
		if (*node == NODE_SPLIT)
			errx(1, "Speicherverwaltung inkonsistent");
		if (*node == NODE_FREE)
			putchar(' ');
		else
			putchar(*node);
	} else if (*node > NODE_FREE) {
		for (unsigned int i = 0; i < node_width_chunks(node); i++) {
			putchar(*node);
		}
	} else {
		memory_dump_recursive(bst_left(node));
		memory_dump_recursive(bst_right(node));
	}
	if (node == bst) {
		putchar('\n');
	}
}

void memory_dump(void)
{
	memory_dump_recursive(bst_root());
}
