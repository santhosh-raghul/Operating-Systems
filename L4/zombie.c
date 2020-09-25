#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>

int main() 
{ 
	printf("Before forking\n");
	sleep(10);

	printf("Forking now\n");
	pid_t pid=fork();

	if(pid==-1)
	{
		printf("Fork failed\n");
		exit(1);
	}
	else if(pid>0)		// parent process
	{
		sleep(10);
		printf("Parent is also ending\n");
	}
	else		// Child process
	{
		printf("Ending child now. Creating zombie process...\n");
		exit(0);
	}
	return 0;
}