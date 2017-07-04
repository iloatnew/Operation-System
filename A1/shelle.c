#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/types.h>
int main(void)
{
	while(1){						/*unendliche Schleife*/	
		if(waitpid(0, NULL, WNOHANG)!=0) 			/*Zombie sammeln*/
			printf("\n Zombie cathched!\n");							
		struct passwd *pwd;
		pwd = getpwuid(getuid());						/*User ID*/
		char name[65];
		gethostname(name, sizeof(name));					/*Rechner ID*/
		char path[100];
		getcwd(path, sizeof(path));						/*Path*/
		printf("%s&%s %s\n", pwd->pw_name, name, path);
		char befehl[20]; char argument[10];
		scanf(" %s",befehl);						/*Befehl & Argument*/
		scanf(" %s",argument);		
		printf("Befehl: %s\nArgument: %s\n", befehl, argument); 	/*Argument soll in '-' anfangen, sonst Felhermeldung*/
		int bkg=0;		
		char *bef= befehl;
		if(*bef=='!'){					/*wenn erste Buchstaben von Befehl '!' ist, bkg und löscht es*/
			bkg=1;
			for(int i=0;i<sizeof(befehl)-1;i++)
				befehl[i]=befehl[i+1];
			}
		else
			bkg=-1;
		pid_t fpid;
/*---------------------------------------------Hintergrund Process--------------------------------------------------------*/ 
		if(bkg==1){		
			printf("\n ist Hintergrund\n");
			fpid=fork();							/*impliziert ein Childprocess*/		
			if(fpid<0)
				printf("fork() Fehler\n");
			else if(fpid==0){						/*falls diese Progress Childprocess ist*/
				sleep(5);
				if(strcmp(befehl,"cd")==0){}				/*Ausnahme: cd wird von Parentprocess durchfuhren*/
				else if(strcmp(befehl,"exit")==0){}
				else if(execlp(befehl,befehl,argument,(char*)0)==-1){	
				/*versuch Befehl zu laufen, ansonsten Fehlermeldung werfen*/
					printf("\nUngultiger Befehl: %s\n",befehl);
				}		
				exit(0);						/*beendet sich, falls Befehl "cd" oder "exit" ist.*/
			}
			else{		  	
					
				if(strcmp(befehl,"cd")==0){				/*change directory*/
					if(chdir(argument)==-1)
						printf("Ungultiges Path\n");
				}
				else if(strcmp(befehl,"exit")==0){			/*beendet die Schleife*/
					return strtol(argument,NULL,10);
	      				
				}
			}
		}
		else{		//Kein Hintergrund process
			printf("\nnicht Hintergrund\n");
			fpid=fork();							/*impliziert ein Childprocess*/		
			if(fpid<0)
				printf("fork() Fehler\n");
			else if(fpid==0){						/*falls diese Progress Childprocess ist*/
				sleep(5);
				if(strcmp(befehl,"cd")==0){}				/*Ausnahme: cd wird von Parentprocess durchfuhren*/
				else if(strcmp(befehl,"exit")==0){}
				else if(execlp(befehl,befehl,argument,(char*)0)==-1){	
				/*versuch Befehl zu laufen, ansonsten Fehlermeldung werfen*/
					printf("\nUngultiger Befehl: %s\n",befehl);
				}		
				exit(0);						/*beendet sich, falls Befehl "cd" oder "exit" ist.*/
			}
			else{		  	
				wait(0);	
				if(strcmp(befehl,"cd")==0){				/*change directory*/
					if(chdir(argument)==-1)
						printf("Ungultiges Path\n");
				}
				else if(strcmp(befehl,"exit")==0){			/*beendet die Schleife*/
					return strtol(argument,NULL,10);
	      				
				}
			}
		}
	}
		return 0;
}
