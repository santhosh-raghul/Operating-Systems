#include<stdio.h>
#include<sys/wait.h>
#include<pthread.h>

#define BUFFER_SIZE 5
#define FULL (in+1)%BUFFER_SIZE==out
#define EMPTY in==out
#define MAX 10

void* producer(void* par);
void* consumer(void* par);

int buf[BUFFER_SIZE],in=0,out=0,done=0;

int main()
{	
	pthread_t tid[2];
	pthread_create(&tid[0],NULL,producer,NULL);
	pthread_create(&tid[1],NULL,consumer,NULL);
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	return 0;
}

void* producer(void* par)
{
	int i=-1;
	while(i<MAX) // producer produces whole numbers from 0 to MAX
	{
		while(FULL); // wait while full - spin wait
		buf[in]=++i; // produce data and put into buffer
		printf("produced data %3d   at index %3d\n",i,in);
		in=(in+1)%BUFFER_SIZE; // update in index
	}
	done=1; // set done to 1 when the producer is done producing
	pthread_exit(0);
}

void* consumer(void* par)
{
	int data;
	while(1)
	{
		while(EMPTY); // wait while empty - spin wait
		data=buf[out]; // consume data from buffer
		printf("consumed data %3d   at index %3d\n",data,out);
		out=(out+1)%BUFFER_SIZE; // update out index
		if(done && EMPTY) // if the producer is done and the buffer has been emptied
			break; // end the loop since all items have been consumed
	}
	pthread_exit(0);
}