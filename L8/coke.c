#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<pthread.h>
#include<semaphore.h>

void* producer_runner(void* param);
void* consumer_runner(void* param);

int coke=5;

sem_t mutex,full,empty;

int main()
{
	int i,a=1,b=2;
	pthread_t prod1,prod2,cons1,cons2;

	sem_init(&mutex,0,1);
	sem_init(&full,0,5);
	sem_init(&empty,0,5);
	
	pthread_create(&prod1,NULL,producer_runner,&a);
	pthread_create(&prod2,NULL,producer_runner,&b);
	pthread_create(&cons1,NULL,consumer_runner,&a);
	pthread_create(&cons2,NULL,consumer_runner,&b);

	pthread_join(prod1,NULL);
	pthread_join(prod2,NULL);
	pthread_join(cons1,NULL);
	pthread_join(cons2,NULL);

	return 0;
}

void* producer_runner(void* param)
{
	int x=*((int*)param);
	while(1)
	{
		sleep(rand()%3);
		sem_wait(&empty);
		sem_wait(&mutex);
		coke++;
		printf("producer %d is producing - increased to %d\n",x,coke);
		sem_post(&full);
		sem_post(&mutex);
	}
}

void* consumer_runner(void* param)
{
	int x=*((int*)param);
	while(1)
	{
		sleep(rand()%3);
		sem_wait(&full);
		sem_wait(&mutex);
		coke--;
		printf("consumer %d is consuming - decreased to %d\n",x,coke);
		sem_post(&empty);
		sem_post(&mutex);
	}
}
