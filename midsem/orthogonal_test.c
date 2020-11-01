#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/sysinfo.h>
#include<sys/time.h>

typedef struct par	// structure to send parameters
{
	float *A,*B,*C;	// A, B are matrices to be multiplied, C is where product is stored
	int m,n,start_row,end_row;	// m,n - order of matrix A
	// start_row - row from which product is to be calculated in the current thread
	// end_row - row till which product is to be calculated in the current thread
}par;

// check whether given matrix is orthogonal or not using multithreading and return flag value
int check_orthogonal(char* filename);

// check whether given matrix is orthogonal or not serially and return flag value
int check_orthogonal_serial(char* filename);

// print message about the orthogonality of the matrix acc to the flag value returned by orthogonality checking functions
void print_message(int flag,char* filename);

// utility function to multiply A, B and store it in product
void multiply_utility(float *A,float *B,float *product,int m,int n);

// runner function to calculate specified rows of the product matrix 
void* matrix_product_runner(void* _param);

// to get time difference in milliseconds between 2 timeval variables
double time_diff(struct timeval begin,struct timeval end);

int main(int argc,char* argv[])
{
	if(argc!=2)
	{
		fprintf(stderr,"%s: missing file operand\nUsage: %s filename\n",argv[0],argv[0]);
		exit(1);
	}
	struct timeval begin, end;
	double multithreaded,serial;

	printf("serial:\n");
	gettimeofday(&begin, 0);
	print_message(check_orthogonal_serial(argv[1]),argv[1]);
	gettimeofday(&end, 0);
	serial=time_diff(begin,end);

	printf("\nmultithreaded:\n");
	gettimeofday(&begin, 0);
	print_message(check_orthogonal(argv[1]),argv[1]);
	gettimeofday(&end, 0);
	multithreaded=time_diff(begin,end);

	printf("\ntime taken for:\n");
	printf("multithreaded: %lfms\n",multithreaded);
	printf("serial:        %lfms\n",serial);
	return 0;
}


int check_orthogonal_serial(char* filename)
{
	FILE* file;
	if((file=fopen(filename,"r"))==NULL)
		return -2;	// if file is not found, return -2

	int m,n,flag=1;
	fscanf(file,"%d %d",&m,&n);	// read matrix dimensions in m,n
	if(m!=n)
		return -1;	// if given matrix is not square, return -1
	// read matrix elements in M[][] and also store the transpose in M_t[][]
	float M[m][n],M_tr[n][m],pd[m][m],sum;
	for(int i=0;i<m;i++)
		for(int j=0;j<n;j++)
		{
			fscanf(file,"%f",&(M[i][j]));
			M_tr[j][i]=M[i][j];
		}
	fclose(file);

	// calculate product matrix
	for(int i=0;i<m;i++)
		for(int j=0;j<m;j++)
		{
			sum=0;
			for(int k=0;k<n;k++)
				sum+=M[i][k]*M_tr[k][j];
			pd[i][j]=sum;
		}

	// print the product matrix
	// printf("the product is:\n");
	// for(int i=0;i<m;i++)
	// {
	// 	printf("\t");
	// 	for(int j=0;j<m;j++)
	// 		printf("%f ",pd[i][j]);
	// 	printf("\n");
	// }

	// check if product has 0 in non diagonal elements and same value in diagonal elements
	for(int i=0;i<m && flag!=0;i++)
	{
		for(int j=0;j<m;j++)
			if((i==j && pd[i][j]!=pd[0][0])||(i!=j && pd[i][j]!=0))
			{
				flag=0;
				break;
			}
	}

	// check if product has 0 in non diagonal elements and same value in diagonal element, check if the diagonal values are 1
	if(flag && pd[0][0]==1)
		flag=2;	// if yes set flag to 2
	return flag;
}

int check_orthogonal(char* filename)
{
	FILE* file;
	if((file=fopen(filename,"r"))==NULL)
		return -2;	// if file is not found, return -2

	int m,n,flag=1;
	fscanf(file,"%d %d",&m,&n);	// read matrix dimensions in m,n
	if(m!=n)
		return -1;	// if given matrix is not square, return -1
	// read matrix elements in M[][] and also store the transpose in M_t[][]
	float M[m][n],M_tr[n][m],pd[m][m];
	for(int i=0;i<m;i++)
		for(int j=0;j<n;j++)
		{
			fscanf(file,"%f",&(M[i][j]));
			M_tr[j][i]=M[i][j];
		}
	fclose(file);

	// calculate product matrix
	multiply_utility((float*)M,(float*)M_tr,(float*)pd,m,n);

	// print the product matrix
	// printf("the product is:\n");
	// for(int i=0;i<m;i++)
	// {
	// 	printf("\t");
	// 	for(int j=0;j<m;j++)
	// 		printf("%f ",pd[i][j]);
	// 	printf("\n");
	// }

	// check if product has 0 in non diagonal elements and same value in diagonal elements
	for(int i=0;i<m;i++)
	{
		for(int j=0;j<m;j++)
			if((i==j && pd[i][j]!=pd[0][0])||(i!=j && pd[i][j]!=0))
			{
				flag=0;
				break;
			}
	}
	// check if product has 0 in non diagonal elements and same value in diagonal element, check if the diagonal values are 1
	if(flag && pd[0][0]==1)
		flag=2;	// if yes set flag to 2
	return flag;
}

void multiply_utility(float *A,float *B,float *product,int m,int n)
{
	int N=get_nprocs_conf(),	// get number of processor cores in N to create N number of threads
		start_row=0,
		start_col=0,
		max=-1;
	par param[N];
	pthread_t tid[N];
	pthread_attr_t attr[N];

	printf("number of CPU cores was found to be %d so creating %d threads\n",N,N);	
	for(int i=0;i<N;i++)
	{
		// setting up parameters
		param[i].A=A;
		param[i].B=B;
		param[i].C=product;
		param[i].m=m;
		param[i].n=n;
		param[i].start_row=start_row;
		start_row+=m/N+(m%N>i?1:0);	// setting up for next thread
		param[i].end_row=start_row-1;

		// refer printf message also
		if(param[i].start_row>param[i].end_row)
		{
			printf("\tthread %d will not calculate anything \n",i+1);
			if(max==-1)	// if ith thread is useless (due to small input size), set max to i
				max=i;
		}
		else
		{
			if(param[i].start_row<param[i].end_row)
				printf("\tthread %d will calculate row %d to row %d of the product \n",i+1,param[i].start_row,param[i].end_row);
			else
				printf("\tthread %d will calculate row %d of the product \n",i+1,param[i].start_row);
			pthread_attr_init(&attr[i]);
			pthread_create(&tid[i],&attr[i],matrix_product_runner,&param[i]);
		}
	}
	if(max==-1)	// if all threads were useful, set max to N
		max=N;
	for(int i=0;i<max;i++)
		pthread_join(tid[i],NULL);	// join all the threads
}

void* matrix_product_runner(void* _param)
{
	float sum=0;
	par param=*(par*)_param;
	for(int i=param.start_row;i<=param.end_row;i++)	// each row from start to end, calculate product
		for(int j=0;j<param.m;j++)
		{
			for(int k=0;k<param.n;k++)
				sum+=(*((param.A+i*param.n)+k))*(*((param.B+k*param.m)+j));
			*((param.C+i*param.m)+j)=sum;
			sum=0;
		}
	pthread_exit(0);
}

void print_message(int flag,char* filename)
{
	switch(flag)
		{
			case -2: perror(filename); break;
			case -1: printf("not square matrix. so not orthogonal\n"); break;
			case 0: printf("not orthogonal\n"); break;
			case 1: printf("not orthogonal but can be made orthogonal by dividing the entire matrix by a constant\n"); break;
			case 2: printf("orthogonal\n"); break;
		}
}

double time_diff(struct timeval begin,struct timeval end)
{
	long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = seconds*1000 + microseconds*1e-3;
	return elapsed;
}