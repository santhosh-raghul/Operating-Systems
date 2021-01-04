#include<stdio.h>
#include<unistd.h>

int main()
{
	printf("Hello world!");
	fflush(stdout);
	sleep(3);
	printf("\rGoodbye world!\n");
	return 0;
}