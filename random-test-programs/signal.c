#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void sig_handler(int signum);

int main()
{
	signal(SIGINT,sig_handler);
	while(1)
	{
		printf("Hello\n");
		sleep(1);
	}
}

void sig_handler(int signum)
{
	printf(" interrupt\n");
	signal(SIGINT,SIG_DFL);
}