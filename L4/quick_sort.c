#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/time.h>

void print_array(int a[],int n);
int* create_shm_array(int n);
void quicksort_parallel(int a[],int s,int e);
void quicksort_serial(int a[],int s,int e);
int quick_sort_pass(int a[],int s,int e);
void swap(int a[],int i,int j);
double time_diff(struct timeval begin,struct timeval end);

int main(int argc, char* argv[])
{
	// getting number of elements through command line argument
	if(argc!=2)
	{
		fprintf(stderr,"quick_sort: missing operand\ncorrect syntax:\n./quick_sort <number_of_elements>\n");
		exit(-1);
	}

	int n=atoi(argv[1]);

	// create shared memory for sorting an array of integers
	int *shm_arr=create_shm_array(n);
	// create array for serial quick sort
	int arr[n];

	// create and print random array
	for(int i=0;i<n;i++)
	{
		arr[i]=rand()%10000;
		shm_arr[i]=arr[i];
	}
	printf("\nOriginal array :\n");
	print_array(arr,n);

	struct timeval begin, end;

    gettimeofday(&begin, 0);
	quicksort_parallel(shm_arr,0,n-1);
    gettimeofday(&end, 0);
	double parallel=time_diff(begin,end);
	printf("Parallel sorted :\n");
	print_array(shm_arr,n);

    gettimeofday(&begin, 0);
	quicksort_serial(arr,0,n-1);
    gettimeofday(&end, 0);
	double serial=time_diff(begin,end);
	printf("Serial sorted :\n");
	print_array(arr,n);

	printf("\ntime taken for:\n");
	printf("parallel: %lfs\n",parallel);
	printf("serial:   %lfs\n\n",serial);
	return 0;
}

int* create_shm_array(int n)
{
	key_t key = IPC_PRIVATE;

	int shm_id;
	size_t shm_size=n*sizeof(int);
	if((shm_id=shmget(key,shm_size,IPC_CREAT | 0666))==-1)
	{
		perror("shmget");
		exit(1);
	}

	int *shm_arr;
	if((shm_arr=shmat(shm_id,NULL,0))==(int *)-1)
	{
		perror("shmat");
		exit(1);
	}

	return shm_arr;
}

void swap(int a[],int i,int j)
{
	if(i==-1||i==j) return;
	int temp=a[i];
	a[i]=a[j];
	a[j]=temp;
}

int quick_sort_pass(int a[],int s,int e)
{
	int i,sw=s-1;
	for(i=s;i<e;i++)
		if(a[i]<a[e])
		{
			sw++;
			swap(a,sw,i);
		}
	swap(a,++sw,e);
	return sw;
}

void quicksort_serial(int a[],int s,int e)
{
	if(s>=e) return;
	int pi=quick_sort_pass(a,s,e);
	quicksort_serial(a,s,pi-1);
	quicksort_serial(a,pi+1,e);

}

void quicksort_parallel(int a[],int s,int e)
{
	if(s>=e) return;

	int pi=quick_sort_pass(a,s,e);

	pid_t pid=fork();
	if(pid==-1)
	{
		perror("fork");
		exit(1);
	}
	else if(pid==0)
	{
		quicksort_parallel(a,s,pi-1);
		exit(0);
	}
	pid=fork();
	if(pid==-1)
	{
		perror("fork");
		exit(1);
	}
	else if(pid==0)
	{
		quicksort_parallel(a,pi+1,e);
		exit(0);
	}
	wait(NULL);
	wait(NULL);
}

void print_array(int a[],int n)
{
	for(int i=0;i<n;i++)
		printf("%4d ",a[i]);
	printf("\n");
}

double time_diff(struct timeval begin,struct timeval end)
{
	long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = seconds + microseconds*1e-6;
	return elapsed;
}