#include <stdio.h>
#include <unistd.h>    
#include <sys/types.h>
int main (void){
	printf("user: %d\n", getuid());
	printf("group: %d\n", getgid());
	return 0;
}
