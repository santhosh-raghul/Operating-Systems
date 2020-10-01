#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<time.h>
#include<sys/wait.h>

int main(int argc,char *argv[])
{
	// accepting matrix order as command line arguments
	if(argc!=5)
	{
		printf("Invalid usage!\nCorrect usage: ./matrix_mult <m1> <n1> <m2> <n2>\nfor order of the matrices - m1 x n1 and m2 x n2\n");
		exit(-1);
	}

	// contains orders of the matrices
	int m1=atoi(argv[1]),
	n1=atoi(argv[2]),
	m2=atoi(argv[3]),
	n2=atoi(argv[4]); 

	// validation
	if(!(m1 && n1 && m2 && n2))
	{
		printf("Invalid usage!\nCorrect usage: ./matrix_mult <m1> <n1> <m2> <n2>\nfor order of the matrices - m1 x n1 and m2 x n2\n");
		exit(-1);
	}

	// check #columns of A == #rows of B
	if(n1!=m2)
	{
		printf("n1 is not equal to m2.\nmatrix multiplication not defined.\n");
		exit(0);
	}

	// a,b are 2 matrices to be multiplied, c is the answer
	int a[m1][n1],b[m2][n2],c[m1][n2];

	// generating random matrix
	srand(time(0));
	printf("generating random matrix...\n");
	for(int i=0;i<m1;i++)
		for(int j=0;j<n1;j++)
			a[i][j]=rand()%100;
	for(int i=0;i<m2;i++)
		for(int j=0;j<n2;j++)
			b[i][j]=rand()%100;

	//printing both the matrices
	printf("matrix A :\n");
	for(int i=0;i<m1;i++)
	{
		for(int j=0;j<n1;j++)
			printf("%6d",a[i][j]);
		printf("\n");
	}
	printf("matrix B :\n");
	for(int i=0;i<m2;i++)
	{
		for(int j=0;j<n2;j++)
			printf("%6d",b[i][j]);
		printf("\n");
	}

	pid_t pid;
	FILE* ans;
	int sum=0;
	// forking inside loop. creating 1 process for each value to be calculated
	for(int i=0;i<m1;i++)
	{
		for(int j=0;j<n2;j++)
		{
			// new process for c[i][j] to be calculated
			pid=fork();
			if(pid==0)
			{
				// for each pair of int i,j 0<=i<m1 and 0<=j<n2, find c[i][j] in sum
				for(int k=0;k<n1;k++)
					sum+=a[i][k]*b[k][j];
				ans=fopen("ans.mat","a"); // for data sharing
				fprintf(ans,"%d %d %d\n",i,j,sum); // put i, j and c[i][j] in file
				// close file and exit child
				fclose(ans);
				exit(0);
			}
		}
	}

	int x,y,z;	
	ans=fopen("ans.mat","r");
	// read answer from file and put it onto memory
	for(int i=0;i<m1;i++)
		for(int j=0;j<n2;j++)
		{
			wait(NULL);
			fscanf(ans,"%d %d %d",&x,&y,&z);
			c[x][y]=z;
		}

	// print the answer
	printf("product :\n");
	for(int i=0;i<m1;i++)
	{
		for(int j=0;j<n2;j++)
			printf("%10d",c[i][j]);
		printf("\n");
	}

	// delete the file used so that it is invisible to the end user
	remove("ans.mat");
	
	return 0;
}