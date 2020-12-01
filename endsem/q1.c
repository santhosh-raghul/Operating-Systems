#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

void* runner(void* param);

sem_t mutex;
int number=2,a[4]={0,0,0,0};

int main()
{
	int i,param[4]={0,1,2,3};
	pthread_t thread_id[4];

	sem_init(&mutex,0,1);
	
	pthread_create(&thread_id[0],NULL,runner,&param[0]);
	pthread_create(&thread_id[1],NULL,runner,&param[1]);
	pthread_create(&thread_id[2],NULL,runner,&param[2]);
	pthread_create(&thread_id[3],NULL,runner,&param[3]);
	
	for(i=0;i<4;i++)
	{
		pthread_join(thread_id[i],NULL);
		printf("\nthread %d printed %d numbers",i,a[i]);
	}

	printf("\n");
	return 0;
}

void* runner(void* param)
{
	int id = *((int*)param);
	while(1)
	{
		sem_wait(&mutex);
		if(number<=54)
		{
			printf("%d ",number);
			a[id]++;
		}
		else
			break;
		number+=2;
		sem_post(&mutex);
	}
	sem_post(&mutex);
	pthread_exit(0);
}