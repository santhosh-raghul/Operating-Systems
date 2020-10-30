#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#define MAX 20

typedef struct parameters
{
	int **mat,temp[MAX][MAX],p,q,n;
}parameters;

int determinant(int mat[][MAX],int N);
void print_int_matrix(int mat[][MAX],int n);
void print_matrix(float mat[][MAX],int n);
void *inv_runner(void* param);
void get_cofactor(int mat[][MAX], int temp[][MAX], int p, int q, int n);
int inverse(int mat[][MAX],float adj[][MAX],int N);
void handle_input(char* filename);

int main(int argc,char* argv[])
{
	if(argc==1)
	{
		fprintf(stderr,"%s: missing file operand\nUsage: %s file1 [file2] [file3]...\n",argv[0],argv[0]);
		exit(1);
	}

	int flag;
	for(int i=1;i<argc;i++)	// handle each input file
		handle_input(argv[i]);

	return 0;
}

void handle_input(char* filename)	// read the file and find the inverse of the matrix in given file
{
	FILE* file;
	if((file=fopen(filename,"r"))==NULL)
	{
		perror(filename);
		return;
	}

	// read matrix dimensions in m,n
	int m,n,flag;
	fscanf(file,"%d %d",&m,&n);
	if(m!=n)
	{
		printf("%s: not a square matrix, inverse doesn't exist\n",filename);
		return;
	}

	// read matrix elements in mat[][]
	int mat[MAX][MAX];
	float inv[MAX][MAX];
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
			fscanf(file,"%d",&(mat[i][j]));
	fclose(file);

	flag=inverse(mat,inv,n);	// inverse returns 1 if inv exists, 0 if it doesn't
	if(flag)
	{
		printf("%s: inverse is :\n",filename);
		print_matrix(inv,n);	// if inv exists, then print the inverse
	}
	else
		printf("%s: not invertible\n",filename);	// if inv doesn't exist, then print message
}

// function for storing inverse of matrix mat in inverse, if exists. returns 0 if mat is not invertible 
int inverse(int mat[][MAX],float inverse[][MAX],int N)
{
	int det=determinant(mat,N); // find determinant of mat
	if (det==0)
		return 0;
	if(N==1) // base case - single element matrix
	{
		inverse[0][0]=1.0/det;
		return 1;
	}
	// temp is used to store cofactors of mat[][]

	int sign = 1, temp[N][N];
	float *pointer;

	pthread_t tid[N][N];
	parameters p[N][N];

	for(int i=0;i<N;i++)
		for(int j=0; j<N; j++)
		{
			// get adj of mat[i][j] using threads
			p[i][j].p=i;p[i][j].q=j;p[i][j].n=N;
			p[i][j].mat=mat;
			pthread_create(&tid[i][j],NULL,inv_runner,&p[i][j]);
		}

	for(int i=0;i<N;i++)
		for(int j=0;j<N;j++)
		{
			sign = ((i+j)%2==0)? 1: -1;	// sign of adj[j][i] positive if sum of row and column indexes is even
			pointer=&inverse[j][i];	// adj is the transpose of cofactor matrix
			pthread_join(tid[i][j],(void*)&pointer);
			inverse[j][i]=sign*(*pointer);	// stored adj value
			inverse[j][i]/=det;	// div adj by det to get inverse
			if(inverse[j][i]==-0)
				inverse[j][i]=0;
			free(pointer);
		}

	return 1;
}

void* inv_runner(void* param)
{
	parameters *p=(parameters*)param;
	get_cofactor((int(*)[MAX])p->mat, p->temp, p->p,p->q, p->n);
	float *det=(float*)malloc(sizeof(float));
	*det=(float)determinant(p->temp, p->n-1);
	pthread_exit((void*)det);
}

// recursive function for finding determinant of nxn matrix
int determinant(int mat[][MAX],int N)
{
	int det=0;
	if(N==1)	// base case, determinant of a single element matrix is the element itself 
		return mat[0][0];

	int cof[MAX][MAX],sign=1; // array for cofactors and int for sign multiplier
	// finding determinant using 1st row
	for(int c= 0;c<N;c++)
	{
		// get cofactor of mat[0][c]
		get_cofactor(mat,cof,0,c,N);
		det +=sign*mat[0][c]*determinant(cof,N-1);
		sign = -sign;
	}
	return det;
}

// function for storing cofactor of nxn matrix mat in temp
void get_cofactor(int mat[][MAX], int temp[][MAX], int p, int q, int n)
{
	int i=0,j=0;
	for(int row=0;row<n;row++)	// for each element of the matrix
	{
		if(row==p)	// ignore pth row
			continue;
		for(int col=0;col<n;col++)
		{
			if(col==q)	// ignore qth row
				continue;
			temp[i][j++]=mat[row][col];
			if(j==n-1)
			{
				j=0;
				i++;
			}
		}
	}
}

void print_int_matrix(int mat[][MAX],int N)
{
	for(int i=0;i<N;i++)
	{
		printf("\t");
		for(int j=0;j<N;j++)
			printf("%7d",mat[i][j]);
		printf("\n");
	}
}

void print_matrix(float mat[][MAX],int N)
{
	for(int i=0;i<N;i++)
	{
		printf("\t");
		for(int j=0;j<N;j++)
			printf("%10g",mat[i][j]);
		printf("\n");
	}
}