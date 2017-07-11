#include "4c.h"
#include <stdio.h>
#include<err.h>
void fast_housekeeping(node_t node);
void buddy_fast_free(void *addr)
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
			fast_housekeeping(node);
		}
	}
}
 void fast_housekeeping(node_t node){
	 node_t pnode = bst_parent(node);
	 while(pnode!=NULL){
		 if(get_node_content(bst_left(pnode)) == 0 && get_node_content(bst_right(pnode))==0){
			 set_node_content(pnode,0);
		 }
		 pnode = bst_parent(pnode);
	 }
}
