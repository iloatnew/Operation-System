#include "4b.h"
#include <stdio.h> /* NULL */
void *rekursiv_alloc(void *addr,char request_id,size_t size);

void *buddy_alloc(char request_id, size_t size)
{
	// man soll eine passende Position für die Anfrage finde.
	if(size == 0 || request_id<=0){
		return NULL;
	}
	node_t node = bst_root();
	size = size_to_chunks(size);
	return rekursiv_alloc(node,request_id,size);
}

void *rekursiv_alloc(void *addr,char request_id,size_t size){
	// already occupied
	if(get_node_content(addr) > 0){
		return NULL;
	}
	// more size than available was requested
	if(node_width_chunks(addr) < size){
		return NULL;
	}
	// addr drop out of bottom
	if(addr == NULL){
		return NULL;
	}
	// node was found
	if(get_node_content(addr)==0 && node_width_chunks(addr)==size){
		set_node_content(addr,request_id);
		return node_start_addr(addr);
	}
	// not fund yet
	void* ret=NULL;
	ret = rekursiv_alloc(bst_left(addr),request_id, size);
	if(ret!=NULL){
		set_node_content(addr,-1);
		return ret;
	}
	ret=rekursiv_alloc(bst_right(addr),request_id,size);
	if(ret!=NULL){
		set_node_content(addr,-1);
		return ret;
	}
	return NULL;
}
