#include<stdio.h>
#include<unistd.h>
#include<wait.h>
#include<stdlib.h>
#include<signal.h>

void handler(int sig)
{
	pid_t pid=wait(NULL);
	printf("\t\tchild %d exited\n",pid);
	signal(SIGCHLD,handler);
}

int main()
{
	int i;
	signal(SIGCHLD,handler);
	for(i=0;i<3;i++)
		switch(fork())
		{
			case 0:
				printf("\tchild %d created\n",getpid());
				exit(0);
		}
	sleep(2);
	sleep(2);
	sleep(2);
	// return 0;
}