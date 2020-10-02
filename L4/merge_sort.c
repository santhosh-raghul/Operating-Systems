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
void mergesort_parallel(int a[],int s,int e);
void mergesort_serial(int a[],int s,int e);
void merge(int a[], int s, int e);
double time_diff(struct timeval begin,struct timeval end);

int main(int argc, char* argv[])
{
	// getting number of elements through command line argument
	if(argc!=2)
	{
		fprintf(stderr,"merge_sort: missing operand\ncorrect syntax:\n./merge_sort <number_of_elements>\n");
		exit(-1);
	}

	int n=atoi(argv[1]);

	// create shared memory for sorting an array of integers
	int *shm_arr=create_shm_array(n);
	// create array for serial merge sort
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
	mergesort_parallel(shm_arr,0,n-1);
    gettimeofday(&end, 0);
	double parallel=time_diff(begin,end);
	printf("Parallel sorted :\n");
	print_array(shm_arr,n);

    gettimeofday(&begin, 0);
	mergesort_serial(arr,0,n-1);
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

void merge(int a[], int s, int e)
{
	int m=(s+e)/2;
	int x[m-s+1];
	int y[e-m];
	for(int i=s;i<=m;i++) x[i-s]=a[i];
	for(int i=m+1;i<=e;i++) y[i-m-1]=a[i];
	int i=0,j=0,k=s;
	while(i<m-s+1 && j<e-m)
	{
		if(x[i]<y[j])
			a[k++]=x[i++];
		else
			a[k++]=y[j++];
	}
	while(i<m-s+1) a[k++]=x[i++];
	while(j<e-m) a[k++]=y[j++];
}

void mergesort_serial(int a[],int s,int e)
{
	if(s==e) return;
	int m=(s+e)/2;
	mergesort_serial(a,s,m);
	mergesort_serial(a,m+1,e);
	merge(a,s,e);
}

void mergesort_parallel(int a[],int s,int e)
{
	pid_t pid;
	if(s==e) return;
	int m=(s+e)/2;

	pid=fork();
	if(pid==-1)
	{
		perror("fork");
		exit(1);
	}
	else if(pid==0)
	{
		mergesort_parallel(a,s,m);
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
		mergesort_parallel(a,m+1,e);
		exit(0);
	}
	wait(NULL);
	wait(NULL);
	merge(a,s,e);
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