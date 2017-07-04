#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <assert.h>

#include "4c.h"

extern char bst[NUM_NODES];

int main(void)
{
	void *addr_a, *addr_b, *addr_e, *addr_i;
	puts("Ausgangssituation (leerer Speicher)");
	memory_dump();

	printf("\nA: Alloziere einen Block (10kiB) -> %p\n",
		addr_a = buddy_alloc('A', 10 * 1024));
	memory_dump();

	printf("\nB: Alloziere einen Block (64kiB) -> %p\n",
		addr_b = buddy_alloc('B', CHUNK_SIZE));
	memory_dump();

	printf("\nC: Alloziere 32 Bloecke (2MiB) -> %p\n",
		buddy_alloc('C', 32 * CHUNK_SIZE));
	memory_dump();

	printf("\nD: Alloziere 32 Bloecke (2MiB), erwarte Nullpointer -> %p\n",
		buddy_alloc('D', 32 * CHUNK_SIZE));
	memory_dump();

	printf("\nE: Alloziere 3 Bloecke (192kiB) -> %p\n",
		addr_e = buddy_alloc('E', 3 * CHUNK_SIZE));
	memory_dump();

	printf("\nF: Alloziere 16 Bloecke (1MiB) -> %p\n",
		buddy_alloc('F', 16 * CHUNK_SIZE));
	memory_dump();

	printf("\nG: Alloziere 16 Bloecke (1MiB), erwarte Nullpointer -> %p\n",
		buddy_alloc('G', 16 * CHUNK_SIZE));
	memory_dump();

	printf("\nGebe E frei\n");
	buddy_fast_free(addr_e);
	memory_dump();

	printf("\nH: Alloziere nichts (0B), erwarte Nullpointer -> %p\n",
		buddy_alloc('H', 0));
	memory_dump();

	printf("\nI: Alloziere einen Block (64kiB) -> %p\n",
		addr_i = buddy_alloc('I', 1 * CHUNK_SIZE));
	memory_dump();

	printf("\nGebe A, B, I frei\n");

	buddy_fast_free(addr_a);
	buddy_fast_free(addr_b);
	buddy_fast_free(addr_i);
	memory_dump();
	assert(bst[3] == 0);

	printf("\nJ: Alloziere 16 Bloecke (1MiB) -> %p\n",
		buddy_alloc('J', 16 * CHUNK_SIZE));
	memory_dump();

	printf("\nK: Alloziere einen Block (1B), erwarte Nullpointer -> %p\n",
		buddy_alloc('K', 1));
	memory_dump();

	puts("\nGebe NULL frei");
	buddy_fast_free(NULL);
	memory_dump();

	return 0;
}
