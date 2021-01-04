#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main()
{
	fclose(fopen("file","w")); // create empty file if file not present already
	int f=open("file",O_WRONLY);
	dup2(f,STDOUT_FILENO);
	printf("printf 1\n");
	write(1,"write 1\n",8);
	printf("printf 2\n");
	write(1,"write 2\n",8);
	close(f);
}