#include "4a.h"
#include <stdlib.h>
#include <stdio.h>
#include<err.h>
void buddy_free(void *addr)
{
	if(addr == NULL){}
	else{
		node_t node = bst_root();
		// has the same start address && node>0(exact the place, not -1）
		while((node_start_addr(node)!=addr || get_node_content(node)<=0) && node!=NULL ){
			if(addr < node_split_addr(node)){
				node = bst_left(node);
			}
			else{
				node = bst_right(node);
			}
		}
		// node was found || the given address is wrong
		if(node == NULL){
			errx(255,"buddy free corruption");
		}
		else{
			set_node_content(node,NODE_FREE);
			bst_housekeeping(bst_root());
		}
	}



}
