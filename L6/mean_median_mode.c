#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<pthread.h>

typedef struct parameters // structure to pass parameters
{
	int *arr,size;
}parameters;

void* mean_runner(void* par);
void* median_runner(void* par);
void* mode_runner(void* par);

int main()
{
	int n;
	printf("Enter no of elements : ");
	scanf("%d",&n);
	// printf("Enter elements one by one : ");
	printf("Random array :");
	int a[n];
	srand(time(0));
	for(int i=0;i<n;i++)
	{
		// scanf("%d",&arr[i]);
		a[i]=rand()%100;
		printf("%d ",a[i]);
	}
	printf("\n");

	parameters param;
	param.arr=a;
	param.size=n;

	pthread_t tid[3];
	pthread_attr_t attr[3];

	pthread_attr_init(&attr[0]);
	pthread_attr_init(&attr[1]);
	pthread_attr_init(&attr[2]);

	pthread_create(&tid[0],&attr[0],mean_runner,&param);
	pthread_create(&tid[1],&attr[1],median_runner,&param);
	pthread_create(&tid[2],&attr[2],mode_runner,&param);

	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	pthread_join(tid[2],NULL);

	return 0;
}

void* mean_runner(void* par)
{
	parameters *param=(parameters*)par;
	int *a=param->arr,size=param->size,sum=0;
	float mean;

	for(int i=0;i<size;i++)
		sum+=a[i];
	mean=((float)sum)/size;	// mean = sum/no_of_elements
	printf("mean = %f\n",mean);

	pthread_exit(0);
}

void* median_runner(void* par)
{
	parameters *param=(parameters*)par;
	int *a=param->arr,size=param->size,temp;

	for(int i=0;i<size;i++)
		for(int j=0;j<(size-i-1);j++)
			if(a[j]>a[j+1])
			{
				temp=a[j];
				a[j]=a[j+1];
				a[j+1]=temp;
			}
	// median is middle element in sorted array
	if(size%2)
		printf("median = %d\n",a[size/2]);
	else
		printf("medians = %d, %d\n",a[size/2-1],a[size/2]);

	pthread_exit(0);
}

void* mode_runner(void* par)
{
	parameters *param=(parameters*)par;
	int *a=param->arr,size=param->size,count,max_count=0,mode;

	// mode is the number that appears most number of times
	for(int i=0;i<size;i++)
	{
		count = 0;
		for(int j=0;j<size;j++)
		{
			if(a[j]==a[i])
			count++;
		}
		if(count>max_count)
		{
			max_count=count;
			mode=a[i];
		}
	}
	printf("mode = %d (displaying one of the modes if there are multiple modes)\n",mode);

	pthread_exit(0);
}