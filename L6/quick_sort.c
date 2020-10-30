#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<pthread.h>

typedef struct param
{
	int *arr,start,end;
}param;

void print_array(int a[],int n);
int quick_sort_pass(int a[],int s,int e);
void swap(int a[],int i,int j);
void quicksort_serial(int a[],int s,int e);
void* quicksort_parallel_runner(void* _parameters);
double time_diff(struct timeval begin,struct timeval end);

int main(int argc,char* argv[])
{
	// getting number of elements through command line argument
	if(argc!=2)
	{
		fprintf(stderr,"merge_sort: missing operand\ncorrect syntax:\n%s <number_of_elements>\n",argv[0]);
		exit(-1);
	}

	int n=atoi(argv[1]);

	// create two arrays,one for serial and one for multithreaded merge sort
	int a[n],b[n];

	// create and print random array
	srand(time(0));
	for(int i=0;i<n;i++)
	{
		a[i]=rand()%10000;
		b[i]=a[i];
	}
	printf("\nOriginal array :\n");
	print_array(a,n);

	struct timeval begin,end;
	double serial,parallel,combo;

	gettimeofday(&begin,0);
	quicksort_serial(a,0,n-1);
    gettimeofday(&end,0);
	serial=time_diff(begin,end);
	printf("Serial sorted :\n");
	print_array(a,n);

	param par;
	par.arr=b;
	par.start=0;
	par.end=n-1;
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);

    gettimeofday(&begin,0);
	pthread_create(&tid,&attr,quicksort_parallel_runner,&par);
	pthread_join(tid,NULL);
    gettimeofday(&end,0);
	parallel=time_diff(begin,end);
	printf("Parallel sorted :\n");
	print_array(b,n);

	printf("\ntime taken for:\n");
	printf("parallel: %lfs\n",parallel);
	printf("serial:   %lfs\n",serial);
	return 0;
}

void* quicksort_parallel_runner(void* _parameters)
{
	param par=*(param*)_parameters;
	if(par.start>=par.end) pthread_exit(0);

	int pi=quick_sort_pass(par.arr,par.start,par.end);

	param a,b;
	a.arr=par.arr;	b.arr=par.arr;
	a.start=par.start;	a.end=pi-1;
	b.start=pi+1;	b.end=par.end;

	pthread_t tid,tid2;
	pthread_attr_t attr,attr2;
	pthread_attr_init(&attr);
	pthread_attr_init(&attr2);
	
	pthread_create(&tid,&attr,quicksort_parallel_runner,&a);
	pthread_create(&tid2,&attr2,quicksort_parallel_runner,&b);

	pthread_join(tid,NULL);
	pthread_join(tid2,NULL);
	
	pthread_exit(0);
}

void quicksort_serial(int a[],int s,int e)
{
	if(s>=e) return;
	int pi=quick_sort_pass(a,s,e);
	quicksort_serial(a,s,pi-1);
	quicksort_serial(a,pi+1,e);
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