#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<time.h>
#include<sys/wait.h>

int main()
{
	int n; // contains order of the matrix
	printf("enter matrix order : ");
	scanf("%d",&n);
	int a[n][n],b[n][n],c[n][n];
	// a,b are 2 matrices to be multiplied, c is the answer

	FILE *af,*bf,*ans;
    af=fopen("a.mat","w"); // using files to share data
    bf=fopen("b.mat","w");

	// generating random matrix and saving in file
	srand(time(0));
	printf("generating random matrix...\n");
	fprintf(af,"%d \n",n);
	fprintf(bf,"%d \n",n);
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			a[i][j]=rand()%100;
			b[i][j]=rand()%100;
			fprintf(af,"%d ",a[i][j]);
			fprintf(bf,"%d ",b[i][j]);
		}
		fprintf(af,"\n");
		fprintf(bf,"\n");
	}
	fclose(af);
	fclose(bf);

	//printing both the matrices
	printf("matrix A :\n");
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
			printf("%6d",a[i][j]);
		printf("\n");
	}

	// note that b[j][i] is printed. this means that transpose of
	// the second matrix has been stored in b for easier calculations
	// and while displaying, we can transpose again so that we get 
	// back the original matrix
	printf("matrix B :\n");
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
			printf("%6d",b[j][i]);
		printf("\n");
	}

	pid_t pid;
	char istr[5],jstr[5];
	// forking inside loop. creating 1 process for every value to be calculated
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
		{
			pid=fork();
			if(pid==0)
			{
				// for each pair of int i,j 0<=i,j<n, find ans[i][j]
				sprintf(istr,"%d",i);
				sprintf(jstr,"%d",j);
				char *args[]={"./multiply",istr,jstr,NULL};
				// mult takes 2 arguments i, j and finds the value of ans[i][j] and puts it in ans.mat
				execvp(args[0],args);
			}
		}
	while(wait(NULL)!=-1); // wait for all values to be computed

	int x,y,z;	
	ans=fopen("ans.mat","r");
	// read answer from file and put it onto memory
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
		{
			fscanf(ans,"%d %d %d",&x,&y,&z);
			c[x][y]=z;
		}

	// print the answer
	printf("product :\n");
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
			printf("%10d",c[i][j]);
		printf("\n");
	}

	// delete all the files used so that it is invisible to the end user.
	// i have commented this for now to see how the files look like.
	// please delete these files before running the program if they exist
	remove("a.mat");
	remove("b.mat");
	remove("ans.mat");
	
	return 0;
}