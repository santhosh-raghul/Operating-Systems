#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<pthread.h>
#include<semaphore.h>

int elves=0,reindeer=0;
sem_t mutex;
sem_t santa_sem,reindeer_sem,elf_sem;

void* santa_runner(void* param);
void* reindeer_runner(void* param);
void* elves_runner(void *param);
void int_sig_handler(int signum);

int main()
{
	signal(SIGINT,int_sig_handler);
	sem_init(&mutex, 0, 1);
	sem_init(&elf_sem, 0, 1);
	sem_init(&santa_sem, 0, 0);
	sem_init(&reindeer_sem, 0, 0);
	int i=0,r[9];
	pthread_t santa_thread,elves_threads[9],reindeer_threads[9];

	pthread_create(&santa_thread,NULL,santa_runner,NULL);
	for(int i=0;i<9;++i)
	{
		r[i]=i;
		pthread_create(&reindeer_threads[i],NULL,reindeer_runner,&r[i]);
		pthread_create(&elves_threads[i],NULL,elves_runner,&r[i]);
	}

	pthread_join(santa_thread,NULL);
	for(int i=0;i<9;++i)
	{
		pthread_join(reindeer_threads[i],NULL);
		pthread_join(elves_threads[i],NULL);
	}

	return 0;
}

void* santa_runner(void* param)
{
	while(1)
	{
		sem_wait(&santa_sem);
		sem_wait(&mutex);
		if(reindeer>=9)
		{
			printf("sleigh ready!! merry christmas!!\n");
			for(int i=0;i<9;++i)
				sem_post(&reindeer_sem);
			reindeer=0;
		}
		else if(elves==3)
			printf("santa is helping the elves\n");
		sem_post(&mutex);
	}
	pthread_exit(0);
}

void* reindeer_runner(void* param)
{
	int* id=(int *) param;
	int reindeer_threads=*id;
	printf("reindeer %d came\n",reindeer_threads);
	while(1)
	{
		sem_wait(&mutex);
		reindeer+=1;
		if(reindeer==9)
			sem_post(&santa_sem);
		sem_post(&mutex);
		sem_wait(&reindeer_sem);
		printf("reindeer %d is getting hitched\n",reindeer_threads);
		sleep(2);
	}
	pthread_exit(0);
}

void* elves_runner(void *param)
{
	int* id=(int *) param;
	int reindeer_threads=*id;
	while(1)
	{
		sem_wait(&elf_sem);
		sem_wait(&mutex);
		elves+=1;
		if(elves==3)
			sem_post(&santa_sem);
		else
			sem_post(&elf_sem);
		sem_post(&mutex);
		printf("elf %d needs help!\n",reindeer_threads);
		sleep(1);

		sem_wait(&mutex);
		elves-=1;
		if(elves==0)
			sem_post(&elf_sem);
		sem_post(&mutex);
		sleep(1);
	}
	pthread_exit(0);
}

void int_sig_handler(int signum)
{
	printf("\nexiting...\n");
	exit(0);
}