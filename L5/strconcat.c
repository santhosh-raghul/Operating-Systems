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
		char string1[size],string2[size],conc[2*size];
		close(p2c[0]);
		close(c2p[1]);
		read(c2p[0],&string2,sizeof(string2));				// recieve string from child
		printf("inside parent.. enter string 1: ");
		scanf("%s",string1);								// get a string input
		printf("recived in parent: '%s'\n",string2);
		// concatenate string1 and string2 and store in conc 
		int l1=strlen(string1),l2=strlen(string2);
		for(int i=0;i<l1;i++)
			conc[i]=string1[i];
		for(int i=0;i<=l2;i++)
			conc[i+l1]=string2[i];
		// concatenaion done
		write(p2c[1],conc,sizeof(conc));					// send concatenated string to parent
		printf("concat done, sent in pipe from parent\n");
	}
	else
	{
		char string[size],conc[2*size];
		printf("\rinside child.. enter string 2: ");
		scanf("%s",string);									// get a string input
		close(c2p[0]);
		close(p2c[1]);
		write(c2p[1],string,sizeof(string));				// send to parent
		printf("'%s' sent in pipe from child\n",string);
		read(p2c[0],&conc,sizeof(conc));					// recieve concatenated string from parent
		printf("recieved concatenated string in child : '%s'\n",conc);
	}
	return 0;
}
