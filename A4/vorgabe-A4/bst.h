#ifndef __BST_H__
#define __BST_H__

#define BUDDY_LEVEL 6
#define NUM_CHUNKS (1 << (BUDDY_LEVEL)) /* 2 ^ BUDDY_LEVEL */
#define NUM_NODES (((NUM_CHUNKS) * 2) - 1)
#define CHUNK_SIZE 65536

#define NODE_FREE 0
#define NODE_SPLIT (-1)

/*
 * Ein Element (bzw. Knoten) des Binärbaums ist ein Zeiger auf einen
 * char, d.h. der Inhalt einer "node_t node"-Variable kann per "*node"
 * ausgelesen und verändert werden.
 */
typedef char* node_t;

// Gibt die Wurzel des Binärbaums zurück
node_t bst_root(void);

// Gibt das linke Kind eines Knotens zurück (oder NULL, falls es kein Kind gibt)
node_t bst_left(node_t node);

// Gibt das rechte Kind eines Knotens (oder NULL) zurück
node_t bst_right(node_t node);

// Gibt den Elternknoten zurück (oder NULL, falls node die Wurzel ist)
node_t bst_parent(node_t node);

/*
 * Stellt nach einer buddy_free()-Operation sicher, dass die Invariante
 * "Ein Knoten enthält genau dann -1, wenn mindestens ein Element des
 * Teilbaums unter ihm belegt ist" erhalten bleibt.
 */
void bst_housekeeping(node_t node);

// Gibt den in einem Knoten gespeicherten Wert zurück
char get_node_content(node_t node);

// Setzt den Wert eines Knotens
void set_node_content(node_t node, char value);

/*
 * Gibt die Ebene eines Knotens zurück. 0 für die Wurzel, BUDDY_LEVEL - 1 für
 * die Blätter.
 */
unsigned int node_level(node_t node);

// Gibt die Breite des einem Knoten zugeordneten Speicherbereichs in Blöcken zurück
unsigned int node_width_chunks(node_t node);

// Gibt die Position eines Knotens in seiner Ebene zurück. 0 == linker Rand
unsigned int node_position(node_t node);

/*
 * Rechnet eine Speichermenge in eine Anzahl von Blöcken um, die von den
 * Knoten im Binärbaum unterstützt wird (d.h. es gibt auf jeden Fall einen
 * Knoten mit node_width_chunks(node) == size_to_chunks(...)). Die angefragte
 * Bytezahl wird dabei ggf. auf den nächsten pasenden Wert aufgerundet.
 */
unsigned int size_to_chunks(unsigned int bytes);

// Gibt die Breite des einem Knoten zugeordneten Speicherbereichs in Byte zurück
unsigned int node_width_bytes(node_t node);

// Gibt die kleinste Adresse des einem Knoten zugeordneten Speicherbereichs zurück
void *node_start_addr(node_t node);

// Gibt die größte Adresse des einem Knoten zugeordneten Speicherbereichs zurück
void *node_end_addr(node_t node);

/*
 * Gibt die kleniste Adresse des Speicherbereichs zurück, der im rechten Kind
 * des Knotens beginnt. D.h.: Kleinere Adresse finden sich im linken Kind,
 * größere (oder gleich große) im rechten.
 */
void *node_split_addr(node_t node);

// Schreibt den Binärbaum ebenenweise auf die Standardausgabe.
void bst_dump(void);

// Schreibt die aktuelle Speicherbelegung auf die Standardausgabe.
void memory_dump(void);

#endif
