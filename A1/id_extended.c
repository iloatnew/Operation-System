#include <stdio.h>
#include<pwd.h>
#include<sys/types.h>



int main (int argc,char *argv[]){
	struct passwd *user;
	if（argc>1){
		struct passwd *getpwnam(const char *name);	
		user = getpwnam(*++argv);
		printf("user: %d\n",user->pw_uid);
		printf("group: %d\n",user->pw_gid);
	}
	else{
	printf uid
}
