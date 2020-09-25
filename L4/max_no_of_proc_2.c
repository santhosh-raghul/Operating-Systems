#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>

int main()
{
	int n=31168; // max no of fork calls 
	// set to 31168 - output of command ulimit -u
	for(int i=0;i<n;i++)
	{
		if(fork()==0)
			exit(1); // if fork succeeds, exit with 1
	}
	int c=0;
	for(int i=0;i<n;i++)
	{
		int exit_status;
		wait(&exit_status); //get exit status
		// exit status will be 255 for exit(1) and 0 for unsuccessful fork
		exit_status/=255;
		c+=exit_status;
		// add exit status to c so that c finally contains no of successful fork
	}
	printf("total number of successful fork calls = %d\n",c);
}