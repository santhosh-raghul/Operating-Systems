#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>

void put_sq_mat_in_file(int **a,int n,char *filename);
void odd_magic_sq(int **a,int n);

int main(int argc,char *argv[])
{
	if(argc!=3)
	{
		fprintf(stderr,"%s: invalid usage\nUsage: %s order output_filename\n",argv[0],argv[0]);
		exit(1);
	}

	int n=atoi(argv[1]);
	if(n<=0)
	{
		fprintf(stderr,"%s: invalid usage\nUsage: %s order output_filename\norder must be a positive integer\n",argv[0],argv[0]);
		exit(1);
	}

	// create shared memory 2d matay
	int shm_id;
	int *shm_row_id=calloc(n,sizeof(int));
	int **mat;
	key_t key=IPC_PRIVATE;
	size_t shm_size=n*sizeof(int*),shm_row_size=n*sizeof(int);
	if((shm_id=shmget(key,shm_size,IPC_CREAT | 0666))==-1)
	{
		perror("shmget");
		exit(1);
	}
	if((mat=shmat(shm_id,NULL,0))==(int **)-1)
	{
		perror("shmat");
		exit(1);
	}
	for(int i=0;i<n;i++)
	{
		if((shm_row_id[i]=shmget(key,shm_row_size,IPC_CREAT | 0666))==-1)
		{
			perror("shmget");
			exit(1);
		}
		if((mat[i]=shmat(shm_row_id[i],NULL,0))==(int *)-1)
		{
			perror("shmat");
			exit(1);
		}
	}

	int print=0;
	// create magic square
	if(n%2==1)
	{
		printf("odd order - linear algorithm, fork not possible, creating magic square using serial algorithm\n");
		odd_magic_sq(mat, n);
		print=1;
	}
	else if(n%4==0)
	{
		printf("order is of the form 4n - creating magic square using parallel algorithm\n");
		print=1;

		// initailize
		for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
			mat[i][j]=(n*i)+j+1;

		pid_t pid[5];

		// top left corner
		pid[0]=fork();
		if(pid[0]==-1)
		{
			perror("fork");
			exit(1);
		}
		else if(pid[0]==0)
		{
			for(int i=0;i<n/4;i++) 
				for(int j=0;j<n/4;j++) 
					mat[i][j]=(n*n+1)-mat[i][j];
			exit(0);
		}
		
		// top right corner
		pid[1]=fork();
		if(pid[1]==-1)
		{
			perror("fork");
			exit(1);
		}
		else if(pid[1]==0)
		{
			for(int i=0;i<n/4;i++) 
				for(int j=3*(n/4);j<n;j++) 
					mat[i][j]=(n*n+1)-mat[i][j];
			exit(0);
		}

		// bottom left corner
		pid[2]=fork();
		if(pid[2]==-1)
		{
			perror("fork");
			exit(1);
		}
		else if(pid[2]==0)
		{
			for(int i=3*n/4;i<n;i++) 
				for(int j=0;j<n/4;j++) 
					mat[i][j]=(n*n+1)-mat[i][j];
			exit(0);
		}

		// bottom right corner
		pid[3]=fork();
		if(pid[3]==-1)
		{
			perror("fork");
			exit(1);
		}
		else if(pid[3]==0)
		{
			for(int i=3*n/4;i<n;i++) 
				for(int j=3*n/4;j<n;j++) 
					mat[i][j]=(n*n+1)-mat[i][j];
			exit(0);
		}

		// centre
		pid[4]=fork();
		if(pid[4]==-1)
		{
			perror("fork");
			exit(1);
		}
		else if(pid[4]==0)
		{
			for(int i=n/4;i<3*n/4;i++) 
				for(int j=n/4;j<3*n/4;j++) 
					mat[i][j]=(n*n+1)-mat[i][j];
			exit(0);
		}

		for(int i=0;i<5;i++)
			waitpid(pid[i],NULL,0);
	}
	else
	{
		printf("number is of the form 4n+2\ncode not yet written\n");
	}

	// print magic square
	if(print)
	{
		put_sq_mat_in_file(mat,n,argv[2]);
		printf("magic square stored in file '%s'\n",argv[2]);
	}
	else
		printf("'%s' not created\n",argv[2]);

	// detach shared memory
	for(int i=0;i<n;i++)
	{
		shmdt(mat[i]);
		shmctl(shm_row_id[i],IPC_RMID,NULL);
	}
	shmdt(mat);
	shmctl(shm_id,IPC_RMID,NULL);

	return 0;
}

void put_sq_mat_in_file(int **a,int n,char *filename)
{
	FILE* file=fopen(filename,"w");
	fprintf(file,"%d %d\n",n,n);
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
			fprintf(file,"%d ",a[i][j]);
		fseek(file,-1, SEEK_CUR);
		fprintf(file,"\n");
	}
	fclose(file);
}

void odd_magic_sq(int **a,int n)
{
	// initialize position for 1
	int i=n/2;
	int j=n-1;
 
	// put all values for 1 to n^2 in the magic square
	for(int num=1;num<=n*n;)
	{
		if(i==-1 && j==n)
		{
			j=n-2;
			i=0;
		}
		else
		{
			if (j == n)
				j=0;
			if (i<0)
				i=n-1;
		}

		if(a[i][j])
		{
			j -= 2;
			i++;
			continue;
		}
		else
			a[i][j]=num++;
 
		j++;
		i--;
	}
}