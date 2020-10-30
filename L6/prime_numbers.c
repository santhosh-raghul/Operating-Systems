#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<pthread.h>

void* prime_check_runner(void* num);

int main(int argc,char* argv[])
{
	if(argc!=2)
	{
		printf("invalid usage\ncorrect usage: %s number\nwhere number is the number until which prime numbers are to be printed\n",argv[0]);
		exit(1);
	}

	int n=atoi(argv[1]);
	printf("Prime numbers from 1 to %d are :\n",n);

	int a[n];
	pthread_t tid[n];
	pthread_attr_t attr[n];

	for(int i=0;i<n;i++)
	{
		a[i]=i+1;
		pthread_attr_init(&attr[i]);
		pthread_create(&tid[i],&attr[i],prime_check_runner,&a[i]); // check whether num is prime. if yes, print
	}

	for(int i=0;i<n;n++)
		pthread_join(tid[i],NULL);

	printf("\n");
	return 0;
}

void* prime_check_runner(void* num)
{
	int n=*(int*)num,flag=1,sq;
	sq=(int)sqrt(n)+1;

	if(n<=1)
		pthread_exit(0);

	// check if prime
	for(int i=2;i<=sq;i++)
		if(n%i==0)
		{
			flag = 0; 
			break; 
		}

	// if prime, print the number
	if(flag) 
		printf("%d ",n);

	pthread_exit(0);
}