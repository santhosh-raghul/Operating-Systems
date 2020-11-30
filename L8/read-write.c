#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>
#include<time.h>

#define NO_WRITERS 5
#define NO_READERS 10

sem_t mutex,writeblock;
int data=0,rcount=0;

void *reader(void *args); // each reader thread reads the data once and displays
void *writer(void *args); // each writer thread increments data by 1

int main()
{
	srand(time(0));
	pthread_t rtid[NO_READERS], wtid[NO_WRITERS];
	pthread_attr_t rattr[NO_READERS],wattr[NO_WRITERS];
	int a[NO_READERS];
	sem_init(&mutex,0,1);
	sem_init(&writeblock,0,1);
	for(int i=0;i<NO_READERS;i++)
	{
		a[i]=i;
		if(i<NO_WRITERS) // create NO_WRITERS no of r,w threads 
		{
			pthread_attr_init(&wattr[i]);
			pthread_create(&wtid[i],NULL,writer,(void *)&a[i]);
			pthread_attr_init(&rattr[i]);
			pthread_create(&rtid[i],NULL,reader,(void *)&a[i]);
		}
		else // just to create some randomness, create the rest of the read threads after sleeping for a random amount of time
		{
			sleep(rand()%5);
			pthread_attr_init(&rattr[i]);
			pthread_create(&rtid[i],NULL,reader,(void *)&a[i]);
		}
	}
	for(int i=0;i<NO_WRITERS;i++)
		pthread_join(wtid[i],NULL);
	for(int i=0;i<NO_READERS;i++)
		pthread_join(rtid[i],NULL);
	return 0;
}

void *reader(void *args)
{
	int f;
	f = *((int*)args);
	sem_wait(&mutex);
	rcount = rcount + 1;
	if(rcount == 1)
		sem_wait(&writeblock);
	sem_post(&mutex);
	printf("Data read by the reader %d is %d\n",f,data);
	srand(time(0)*f);
	sleep(1);
	sem_wait(&mutex);
	rcount = rcount - 1;
	if(rcount == 0)
		sem_post(&writeblock);
	sem_post(&mutex);
}

void *writer(void *args)
{
	int f,t;
	f=*((int*)args);
	sleep(rand()%5); // sleeps for a random amount of time
	sem_wait(&writeblock);
	data++;
	printf("Data written by the writer %d is %d\n",f,data);
	sem_post(&writeblock);	
}
