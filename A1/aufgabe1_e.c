#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
int main(void)
{
	struct passwd *pwd;
	pwd = getpwuid(getuid());				/*User ID*/
	char name[HOST_NAME_MAX + 1];
	gethostname(name, sizeof(name));			/*Rechner ID*/
	char path[PATH_MAX + 1];				/*Path*/
	while(1){						/*unendliche Schleife*/
		waitpid(0, NULL, WNOHANG); 			/*Zombie sammeln*/
			
		getcwd(path, sizeof(path));
		printf("%s&%s %s", pwd->pw_name, name, path);
		char befehl[256]; char argument[256];
		scanf(" %s",befehl);				/*Befehl & Argument*/
		scanf(" %s",argument);		
		if (getchar() != '\n') {
			printf("Fehlerhafte Befehleingabe!\n");
			while (getchar() != '\n'); 
			continue;
		}
		printf("Befehl: %s\nArgument: %s\n", befehl, argument);
		int bkg=0;		
		char *bef= befehl;
		if(*bef=='!'){					/*wenn erste Buchstaben von Befehl '!' ist, bkg und löscht es*/
			bkg=1;
			for(int i=0;i<sizeof(befehl)-1;i++)
				befehl[i]=befehl[i+1];
		}
		
		if (strcmp(befehl, "cd") == 0) {
			if (chdir(argument) == -1) {
				perror("shell");
			}
			continue;
		} 	
		if (strcmp(befehl, "exit") == 0) {
			wait(NULL);
			return strtol(argument,NULL,10);
			
		}		
		pid_t fpid;
		fpid=fork();							/*impliziert ein Childprocess*/
		if(fpid<0)
			printf("fork() Fehler\n");
		else if(fpid==0){					/*falls diese Progress Childprogress ist*/
			if(execlp(befehl,befehl,argument,NULL)==-1){	/*versuch Befehl zu laufen, ansonsten Fehlermeldung werfen*/
				perror("shell");
				exit(EXIT_FAILURE);
			}
			exit(0);
		}
		else{
			if(bkg) {
				continue;
			}
			/*wait(0);*/
		  	waitpid(fpid,NULL,0); 						/*warten bis Endung von Childprocess*/	
		}
	}
	return 0;
}
