#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<pthread.h>
#include<semaphore.h>

#define N 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT (ph_num+N-1)%N
#define RIGHT (ph_num+1)%N

sem_t mutex,S[N];
int state[N];

void* philosopher_runner(void*);
void take_chopstick(int);
void put_chopstick(int);
void test(int);
void int_sig_handler(int);

int main()
{
	signal(SIGINT,int_sig_handler);
	int i,phil_num[N];
	pthread_t thread_id[N];

	sem_init(&mutex,0,1);
	for(i=0;i<N;i++)
		sem_init(&S[i],0,0);

	for(i=0;i<N;i++)
	{
		phil_num[i]=i;
		pthread_create(&thread_id[i],NULL,philosopher_runner,&phil_num[i]);
		printf("philosopher %d is thinking\n",i+1);
	}

	for(i=0;i<N;i++)
		pthread_join(thread_id[i],NULL);

	return 0;
}

void* philosopher_runner(void* num)
{
	while(1)
	{
		int *i=num;
		sleep(1);
		take_chopstick(*i);
		sleep(0);
		put_chopstick(*i);
	}
}

void take_chopstick(int ph_num)
{
	sem_wait(&mutex);
	state[ph_num]=HUNGRY;
	printf("philosopher %d is hungry\n",ph_num+1);
	test(ph_num);
	sem_post(&mutex);
	sem_wait(&S[ph_num]);
	sleep(1);
}

void test(int ph_num)
{
	if(state[ph_num]==HUNGRY && state[LEFT]!=EATING && state[RIGHT]!=EATING)
	{
		state[ph_num]=EATING;
		sleep(2);
		printf("philosopher %d is taking chopsticks %d and %d\n",ph_num+1,ph_num+1,LEFT+1);
		printf("philosopher %d is eating\n",ph_num+1);
		sem_post(&S[ph_num]);
	}
}

void put_chopstick(int ph_num)
{
	sem_wait(&mutex);
	state[ph_num]=THINKING;
	printf("philosopher %d is putting chopsticks %d and %d down\n",ph_num+1,ph_num+1,LEFT+1);
	printf("philosopher %d is thinking\n",ph_num+1);
	test(LEFT);
	test(RIGHT);
	sem_post(&mutex);
}

void int_sig_handler(int signum)
{
	printf("\nexiting...\n");
	exit(0);
}