#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>

int main(int argc, char* argv[])
{
	// getting filename through command line argument
	if(argc!=2)
	{
		printf("histogram: missing operand\ncorrect syntax:\n ./histogram filename\n");
		exit(-1);
	}

	FILE* file=fopen(argv[1],"r"); // open the given file
	if(file==NULL)
	{
		printf("histogram: error opening '%s'\n",argv[1]);
		exit(-1);
	}
	fclose(file);

	// create shared memory for counting the frequency of all printable ascii characters
	// count[0] to count[94] will have the count of ascii values 32 to 126
	// and count[95] will have the count of other characters
	key_t key = IPC_PRIVATE;

	int shm_id;
	size_t shm_size=96*sizeof(int);
	if((shm_id=shmget(key,shm_size,IPC_CREAT | 0666))==-1)
	{
		perror("shmget");
		exit(1);
	}

	int *count;
	if((count=shmat(shm_id,NULL,0))==(int *)-1)
	{
		perror("shmat");
		exit(1);
	}

	// count the frequency of each character in one child
	pid_t pid;
	int c;
	for(int i=0;i<95;i++)
	{
		pid=fork(); // 1 fork for each ascii character
		if(pid==-1)
		{
			perror("fork");
			exit(-1);
		}
		else if(pid==0) // child
		{
			file=fopen(argv[1],"r");
			count[i]=0;
			while((c=fgetc(file))!=EOF)
				if(c==(i+32))
					count[i]++;
			fclose(file);
			exit(0);
		}
	}

	// one more fork for counting other characters
	pid=fork();
	if(pid==-1)
	{
		perror("fork");
		exit(-1);
	}
	else if(pid==0)
	{
		file=fopen(argv[1],"r");
		count[95]=0;
		while((c=fgetc(file))!=EOF)
			if(c<32 || c>127)
				count[95]++;
		fclose(file);
		exit(0);
	}


	// wait for all child processes to complete
	while(wait(NULL)!=-1);

	// print histogram
	int sum=count[0]+count[95];
	if(count[0])
		printf("    %10d\t  (space)\n",count[0]);
	for(int i=1;i<95;i++)
		if(count[i])
		{
			sum+=count[i];
			printf("  %c %10d\n",i+32,count[i]);
		}
	if(count[95])
		printf("others %10d\t\n",count[95]);
	
	printf("total  %10d\n",sum);
	return 0;
}