#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <limits.h>
int main(void)
{
	struct passwd *pwd;
	pwd = getpwuid(getuid());				/*User ID*/
	char name[HOST_NAME_MAX + 1];
	gethostname(name, sizeof(name));			/*Rechner ID*/
	char path[PATH_MAX + 1];
	getcwd(path, sizeof(path));				/*Path*/
	while(1){						/*unendliche Schleife*/
			
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
		
	}
	return 0;
}
