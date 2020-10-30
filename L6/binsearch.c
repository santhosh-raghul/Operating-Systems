#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>

typedef struct param
{
	int *arr,start,end,key;
}param;

void* bin_search_runner(void* parameters);

int main()
{
	int n,key,count;
	srand(time(0));
	printf("Implementation of binary search.\nEnter no of elements : ");
	scanf("%d",&n);
	// printf("Enter elements one by one : ");
	printf("Random array :");
	int arr[n];
	for(int i=0;i<n;i++)
	{
		// scanf("%d",&arr[i]);
		arr[i]=rand()%10;
		printf("%d ",arr[i]);
	}
	printf("\nEnter element to be searched : ");
	scanf("%d",&key);

	param a;
	a.arr=arr;
	a.start=0;
	a.end=n-1;
	a.key=key;

	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	pthread_create(&tid,&attr,bin_search_runner,&a);
	pthread_join(tid,NULL);

	return 0;
}

void* bin_search_runner(void* parameters)
{
	param par=*(param*)parameters;
	if(par.start>par.end)	// exit condition
		pthread_exit(0);

	int mid=(par.start+par.end)/2;
	if(par.arr[mid]==par.key)	// if element is found, print message
		printf("%d found at index %d\n",par.key,mid);

	param a,b;
	a.arr=par.arr;	b.arr=par.arr;
	a.key=par.key;	b.key=par.key;
	a.start=par.start;	a.end=mid-1;	// 1st half
	b.start=mid+1;	b.end=par.end;		// 2nd half

	pthread_t tid1,tid2;
	pthread_attr_t attr1,attr2;
	pthread_attr_init(&attr1);
	pthread_attr_init(&attr2);
	
	pthread_create(&tid1,&attr1,bin_search_runner,&a);	// search 1st half
	pthread_create(&tid2,&attr2,bin_search_runner,&b);	// search 2nd half

	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);

	pthread_exit(0);
}