#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>

struct shm_2d_array
{
	int r,c;
	int **arr;
	int shm_id;
	int *shm_row_id;
};

int check_magic_square(char* filename);
int* create_shm_array(int n);
struct shm_2d_array create_shm_2d_array(int r,int c);
void free_shm_2d_array(struct shm_2d_array shm_arr);
void quicksort(int a[],int s,int e);
int quick_sort_pass(int a[],int s,int e);
void swap(int a[],int i,int j);

int main(int argc, char* argv[])
{
	if(argc==1)
	{
		fprintf(stderr,"%s: missing file operand\nUsage: %s file1 [file2] [file3]...\n",argv[0],argv[0]);
		exit(1);
	}

	int flag;
	for(int i=1;i<argc;i++)
	{
		flag=check_magic_square(argv[i]);
		switch (flag)
		{
			case 1:
				printf("%s: magic square\n",argv[i]);
				break;
			case 0:
				printf("%s: not a magic square - sums dont match\n",argv[i]);
				break;
			case -1:
				printf("%s: not a magic square - contains repeated or zero or negative elements\n",argv[i]);
				break;
			case -2:
				printf("%s: not a magic square - not a square matrix\n",argv[i]);
				break;
			case -3:
				perror(argv[i]);
				break;
		}
	}

	return 0;
}

int check_magic_square(char* filename)
{
	// check if file exist and open file
	FILE* file;
	if((file=fopen(filename,"r"))==NULL)
		return -3;
	
	// read matrix dimensions and validate
	int m,n;
	fscanf(file,"%d %d",&m,&n);
	if(m!=n)
		return -2;

	// create shm array and store the matrix and close the file
	struct shm_2d_array mat=create_shm_2d_array(n,n);
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
			fscanf(file,"%d",&(mat.arr[i][j]));
	fclose(file);

	// for(int i=0;i<n;i++)
	// {
	// 	for(int j=0;j<n;j++)
	// 		printf("%10d  ",mat.arr[i][j]);
	// 	printf("\n");
	// }

	// chk if entries are unique
	int arr[n*n];
	for(int i=0;i<n*n;i++)
		arr[i]=mat.arr[i/n][i%n];

	quicksort(arr,0,n*n-1);

	if(arr[0]<=0)
		return -1;
	for(int i=1;i<n*n;i++)
		if(arr[i]==arr[i-1] || arr[i]<=0)
			return -1;

	// create shm array and store sum in it
	// 2n+2 processes --> n for col, n for row, 2 for diagonal
	// sums[0 to n-1] - contains row sums
	// sums[n to 2n-1] - contains col sums
	// sums[2n] and sums[2n+1] - contains diag sums
	int *sums=create_shm_array(2*n+2);
	pid_t pid;
	pid=fork();
	if(pid==-1)
	{
		perror("fork");
		exit(1);
	}
	if(pid>0)
		waitpid(pid,NULL,0);
	else
	{
		// calc row sum
		for(int i=0;i<n;i++)
		{
			pid=fork();
			if(pid==-1)
			{
				perror("fork");
				exit(1);
			}
			if(pid==0)
			{
				int sum=0;
				for(int j=0;j<n;j++)
					sum+=mat.arr[i][j];
				sums[i]=sum;
				exit(0);
			}
		}
		// calc col sum
		for(int i=0;i<n;i++)
		{
			pid=fork();
			if(pid==-1)
			{
				perror("fork");
				exit(1);
			}
			if(pid==0)
			{
				int sum=0;
				for(int j=0;j<n;j++)
					sum+=mat.arr[j][i];
				sums[i+n]=sum;
				exit(0);
			}
		}
		// calc left to right diagonal sum
		pid=fork();
		if(pid==-1)
		{
			perror("fork");
			exit(1);
		}
		if(pid==0)
		{
			int sum=0;
			for(int j=0;j<n;j++)
				sum+=mat.arr[j][j];
			sums[2*n]=sum;
			exit(0);
		}
		// calc right to left diagonal sum
		pid=fork();
		if(pid==-1)
		{
			perror("fork");
			exit(1);
		}
		if(pid==0)
		{
			int sum=0;
			for(int j=0;j<n;j++)
				sum+=mat.arr[j][j];
			sums[2*n+1]=sum;
			exit(0);
		}
		// parent must wait for calc of all sums
		while(wait(NULL)!=-1);
		exit(0);
	}

	// for(int i=0;i<(2*n+2);i++)
	// 	printf("%d ",sums[i]);
	// printf("\n");

	// check if sums are same
	int flag=1;
	for(int i=1;i<(2*n+2);i++)
		if(sums[i]!=sums[0])
		{
			flag=0;
			break;
		}

	free_shm_2d_array(mat);
	return flag;
}

struct shm_2d_array create_shm_2d_array(int r,int c)
{
	struct shm_2d_array *shm_arr=(struct shm_2d_array*)malloc(sizeof(struct shm_2d_array));
	shm_arr->shm_row_id=calloc(c,sizeof(int));

	key_t key = IPC_PRIVATE;

	size_t shm_size=r*sizeof(int*),shm_row_size=r*sizeof(int);
	if((shm_arr->shm_id=shmget(key,shm_size,IPC_CREAT | 0666))==-1)
	{
		perror("shmget");
		exit(1);
	}

	if((shm_arr->arr=shmat(shm_arr->shm_id,NULL,0))==(int **)-1)
	{
		perror("shmat");
		exit(1);
	}

	for(int i=0;i<c;i++)
	{
		if((shm_arr->shm_row_id[i]=shmget(key,shm_row_size,IPC_CREAT | 0666))==-1)
		{
			perror("shmget");
			exit(1);
		}

		if((shm_arr->arr[i]=shmat(shm_arr->shm_row_id[i],NULL,0))==(int *)-1)
		{
			perror("shmat");
			exit(1);
		}
	}

	shm_arr->r=r;
	shm_arr->c=c;

	return *shm_arr;
}

void free_shm_2d_array(struct shm_2d_array shm_arr)
{
	for(int i=0;i<shm_arr.r;i++)
	{
		shmdt(shm_arr.arr[i]);
		shmctl(shm_arr.shm_row_id[i],IPC_RMID,NULL);
	}
	shmdt(shm_arr.arr);
	shmctl(shm_arr.shm_id,IPC_RMID,NULL);
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

void quicksort(int a[],int s,int e)
{
	if(s>=e) return;
	int pi=quick_sort_pass(a,s,e);
	quicksort(a,s,pi-1);
	quicksort(a,pi+1,e);
}

void swap(int a[],int i,int j)
{
	if(i==-1||i==j) return;
	int temp=a[i];
	a[i]=a[j];
	a[j]=temp;
}