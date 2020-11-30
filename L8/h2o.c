#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<pthread.h>
#include<semaphore.h>

int bc=0,h=0,o=0,btotal;
sem_t sbarrier,bmutex,hydroq,oxyq,mutex;

void bond();
void *oxygen_runner(void *arg);
void *hydrogen_runner(void *arg);
void barrier_wait();
void barrier(int n);
void int_sig_handler(int);

int main()
{
	signal(SIGINT,int_sig_handler);
	pthread_t o_thread,h_thread_1,h_thread_2;
	sem_init(&mutex,0,1);
	sem_init(&oxyq,0,0);
	sem_init(&hydroq,0,0);
	sem_init(&sbarrier,0,0);
	sem_init(&bmutex,0,1);
	btotal=3;
	pthread_create(&o_thread,NULL,oxygen_runner,NULL);
	pthread_create(&h_thread_1,NULL,hydrogen_runner,NULL);
	pthread_create(&h_thread_2,NULL,hydrogen_runner,NULL);
	while(1);
}

void *oxygen_runner(void *arg)
{
	while(1)
	{
		sem_wait(&mutex);

		o+=1;
		if(h>=2)
		{
			sem_post(&hydroq);
			sem_post(&hydroq);
			h-=2;
			sem_post(&oxyq);
			o-=1;
		}
		else
			sem_post(&mutex);

		sem_wait(&oxyq);
		printf("1 oxygen atom ready\n");
		bond();
		barrier_wait();
		sem_post(&mutex);
	}
}

void *hydrogen_runner(void *arg)
{
	while(1)
	{
		sem_wait(&mutex);
		h+=1;

		if(h>=2 && o>=1)
		{
			sem_post(&hydroq);
			sem_post(&hydroq);
			h-=2;
			sem_post(&oxyq);
			o-=1;
		}
		else
			sem_post(&mutex);

		sem_wait(&hydroq);
		printf("1 hydrogen atom ready\n");
		bond();
		barrier_wait();
	}
}

void bond()
{
	static int i = 0;
	i++;
	if(i%3 == 0)
		printf("water molecule %d created\n",i/3);
	sleep(2);
}

void barrier_wait()
{
	sem_wait(&bmutex);
	bc++;
	sem_post(&bmutex);
	if(bc == 3)
		sem_post(&sbarrier);
	sem_wait(&sbarrier);
	sem_post(&sbarrier);
}

void int_sig_handler(int signum)
{
	printf("\nexiting...\n");
	exit(0);
}