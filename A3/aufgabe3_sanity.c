#include "vorgabe3.h"

/****************************************/
/* Hier die Bonusaufgabe implementieren */
/****************************************/

// before this function was called, mm.av should be already locked

int check_save_allocate(int amount)
{
	//Check to see if the amount given would safely fit within the available memory
	return mm.available > amount;
}
