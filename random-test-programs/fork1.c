#include<stdio.h>
#include<unistd.h>

int main()
{
	FILE *fp;
    fp=fopen("op","w");
	if(fork() && fork())
		fork();
	if(fork() || fork())
	{
		fork();
		// fork();
	}
	fprintf(fp,"abc\n");
	fclose(fp);
}