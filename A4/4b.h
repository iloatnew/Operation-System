#ifndef __4B_H__
#define __4B_H__

#include "4a.h"
#include <stdlib.h> /* size_t */

void *buddy_alloc(char request_id, size_t size);

#endif
