#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<pthread.h>

typedef struct parameters // structure to pass parameters
{
	int *arr,size;
}parameters;

void* asc_sort_runner(void* par);
void* desc_sort_runner(void* par);

int main()
{
	int n;
	printf("Enter no of elements : ");
	scanf("%d",&n);
	// printf("Enter elements one by one : ");
	printf("Random array :");
	int a[n],b[n];
	srand(time(0));
	for(int i=0;i<n;i++)
	{
		// scanf("%d",&arr[i]);
		a[i]=rand()%100;
		b[i]=a[i];
		printf("%d ",a[i]);
	}
	printf("\n");

	parameters par_asc,par_desc;
	par_asc.arr=a;
	par_asc.size=n;
	par_desc.arr=b;
	par_desc.size=n;

	pthread_t tid[2];
	pthread_attr_t attr[2];
	pthread_attr_init(&attr[0]);
	pthread_attr_init(&attr[1]);
	pthread_create(&tid[0],&attr[0],asc_sort_runner,&par_asc); // sort in asc and print
	pthread_create(&tid[1],&attr[1],desc_sort_runner,&par_desc); // sort in desc and print
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
}

void* asc_sort_runner(void* par)
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
	printf("Array after asc sorting : ");
	for(int i=0;i<size;i++)
		printf("%d ",a[i]);
	printf("\n");
}

void* desc_sort_runner(void* par)
{
	parameters *param=(parameters*)par;
	int *a=param->arr,size=param->size,temp;
	for(int i=0;i<size;i++)
		for(int j=0;j<(size-i-1);j++)
			if(a[j]<a[j+1])
			{
				temp=a[j];
				a[j]=a[j+1];
				a[j+1]=temp;
			}
	printf("Array after desc sorting : ");
	for(int i=0;i<size;i++)
		printf("%d ",a[i]);
	printf("\n");
}