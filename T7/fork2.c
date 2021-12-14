#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

int main(int argc, char* argv){
	char* param[4];
	param[0] = "morphed";
	param[1] = "55";
	param[2] = "Second Command Parameter";
	param[3] = "Third Command Paramter";
	param[4] = NULL;

	int pid;
	int cpid;
	int status;
	
	pid = fork();
	if(pid == 0){
		//Child
		int rc;
		rc = execv("./morphed", param);
		printf("BAD\n");
	}else{
		//Parent
		printf("Parent program\n");
		cpid = wait(&status);
		if(WIFEXITED(status)){
			printf("Child program terminated with: %d \n", WEXITSTATUS(status));
		}
	}
	return 0;
}
