#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

int sum;
void* runner(void* par);

int main(int argc, char* argv[])
{
	pthread_t tid;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_create(&tid,&attr,runner,argv[1]);
	pthread_join(tid,NULL);

	printf("sum=%d\n",sum);
}

void* runner(void* par)
{
	int i,n=atoi((char*)par);
	sum=0;
	for(i=1;i<=n;i++)
		sum+=i;
	printf("sum = %d\n",sum);
	char *args[]={"ls",NULL};
	execvp(args[0],args);
	pthread_exit(0);
}