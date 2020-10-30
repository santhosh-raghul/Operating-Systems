#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<time.h>
#include<sys/wait.h>
#define size 64

int main(int argc,char *argv[])
{
	int p2c[2],c2p[2];
	if(pipe(p2c)==-1||pipe(c2p)==-1)
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
		char string[size],rev[size];
		printf("inside parent.. enter string: ");
		scanf("%[^\n]%*c",string);					// get a string input delimitted by \n
		close(p2c[0]);
		close(c2p[1]);
		write(p2c[1],string,sizeof(string));		// send to child
		printf("'%s' sent in pipe\n",string);
		read(c2p[0],&rev,sizeof(rev));				// recieve string from parent
		printf("recieved reverse in parent : '%s'\n",rev);
	}
	else
	{
		char string[size],rev[size];
		close(c2p[0]);
		close(p2c[1]);
		read(p2c[0],&string,sizeof(string));		// recieve string from parent
		printf("recived in child: '%s'\n",string);
		// store reverse of string in the char array rev
		int index=strlen(string)-1;
		for(int i=index;i>=0;i--)
			rev[index-i]=string[i];
		rev[index+1]='\0';
		// reversal done
		write(c2p[1],rev,sizeof(rev));				// send reverse to parent
		printf("reversal done, sent in pipe\n");
	}
	return 0;
}
