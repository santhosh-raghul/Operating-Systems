#include<stdio.h>
#include<sys/wait.h>
#include<pthread.h>

#define BUFFER_SIZE 5
#define FULL (in+1)%BUFFER_SIZE==out
#define EMPTY in==out
#define MAX 10
#define PRODUCER 0
#define CONSUMER 1
#define FALSE 0
#define TRUE 1

void* producer(void* par);
void* consumer(void* par);

int in=0,out=0,turn=0,flag[2]={0,0},buf[BUFFER_SIZE],end=FALSE;

int main()
{	
	printf("note: actual order of execution may vary from the order of printf\n");
	pthread_t tid[2];
	pthread_create(&tid[0],NULL,producer,NULL);
	pthread_create(&tid[1],NULL,consumer,NULL);
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	return 0;
}

void* producer(void* par)
{
	int i=-1,index;
	while(i<MAX) // producer produces whole numbers from 0 to MAX
	{
	// wait while full
		while(FULL);
	// petersons solution lock for mutual exclusion
		flag[PRODUCER]=TRUE;
		turn=CONSUMER;
		while (flag[CONSUMER] && turn==CONSUMER);
	// critical section - produce data in this case
		buf[in]=++i;
		index=in; // save index for printing
		in=(in+1)%BUFFER_SIZE;
	// unlock
		flag[PRODUCER]=FALSE;
	// remainder section
		printf("produced data %3d   at index %3d\n",i,index);
	}
	pthread_exit(0);
}

void* consumer(void* par)
{
	int data,index;
	while(data<MAX)
	{
	// wait while empty
		while(EMPTY);
	// petersons solution lock for mutual exclusion
		flag[CONSUMER]=TRUE;
		turn=PRODUCER;
		while (flag[PRODUCER] && turn==PRODUCER);
	// critical section - consume data in this case
		data=buf[out];
		index=out; // save index for printing
		out=(out+1)%BUFFER_SIZE;
	// unlock
		flag[CONSUMER]=FALSE;
	// remainder section
		printf("consumed data %3d   at index %3d\n",data,index);
		if(EMPTY && end)
			break;
	}
	pthread_exit(0);
}