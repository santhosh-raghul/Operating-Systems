#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>

int main() 
{ 
	printf("Before forking\n");
	sleep(10);

	pid_t pid=fork();

	if(pid==-1)
	{
		printf("Fork failed\n");
		exit(1);
	}
	else if(pid>0)		// parent process
	{
		printf("Forking done\n");
		sleep(10);
		printf("Ending parent now. Creating orphan process...\n");
		exit(0);
	}
	else		// Child process
	{
		sleep(20);
		printf("\nChild is ending only now\n");
	}
	return 0;
}