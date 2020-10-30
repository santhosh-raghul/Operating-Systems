#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<time.h>
#include<sys/wait.h>
#define size 64

void substring(char* string,int start,int len);

int main(int argc,char *argv[])
{
	int p2c[2];
	if(pipe(p2c)==-1)
	{
		perror("pipe");
		exit(1);
	}

	pid_t pid=fork();

	if(pid==-1)
	{
		perror("fork");
		exit(1);
	}
	else if(pid>0)
	{
		char string[size];
		int start,len;
		printf("inside parent.. enter string: ");
		scanf("%s",string);							// get a string
		close(p2c[0]);
		printf("inside parent.. enter starting index: ");
		scanf("%d",&start);							// get starting index
		printf("inside parent.. enter length: ");
		scanf("%d",&len);							// get length
		write(p2c[1],string,sizeof(string));		// send string to child
		write(p2c[1],&start,sizeof(start));			// send starting index to child
		write(p2c[1],&len,sizeof(len));				// send length to child
		printf("'%s' sent in pipe\n",string);
	}
	else
	{
		char string[size];
		int start,len;
		close(p2c[1]);
		read(p2c[0],&string,sizeof(string));		// recieve string from parent
		read(p2c[0],&start,sizeof(start));			// recieve starting index from parent
		read(p2c[0],&len,sizeof(len));				// recieve length from parent
		printf("recived in child:\nstring: '%s', starting index: %d, length: %d\n",string,start,len);
		substring(string,start,len);
	}
	return 0;
}

void substring(char* string,int start,int len)
{
	char substr[len];
	int i;
	if(start+len>strlen(string)||start<0)
	{
		printf("index is out of range\n");
		return;
	}
	for(i=0;i<len;i++)
		substr[i]=string[i+start];
	substr[i]='\0';
	printf("substring is: '%s'\nc",substr);
}