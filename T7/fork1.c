

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

int main()
{ 
    int pid;
	int cpid;
	int status;
    int i=0;
    
    printf("Ready to fork...\n"); 
    pid = fork();	// create a clone 
	
	if(pid != 0){
		cpid = wait(&status);
		printf("CPID: %d STATUS: %d \n", cpid, status);

		if(WIFEXITED(status)){
			printf("Child has returned the value: %d\n", WEXITSTATUS(status));
		}
	}

	printf("Fork returned %d\n",pid);
	
	return 0;


}

