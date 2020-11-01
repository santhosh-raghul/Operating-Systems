#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<fcntl.h>

int main(int argc,char* argv[])
{
	if(argc!=6)
	{
		fprintf(stderr,"%s: invalid usage\nUsage: %s m n min max filename\nto generate a mxn matrix with entries ranging from min to max and store it in the specified file\n",argv[0],argv[0]);
		exit(1);
	}

	int m=atoi(argv[1]),
		n=atoi(argv[2]),
		min=atoi(argv[3]),
		max=atoi(argv[4]),
		file;

	fclose(fopen(argv[5],"w"));	// create file if it doesn't already exist
	file=open(argv[5],O_WRONLY);
	dup2(file,1);
	printf("%d %d",m,n);
	srand(time(0));
	for(int i=0;i<m;i++)
	{
		printf("\n");
		for(int j=0;j<n;j++)
			printf("%d ",min+rand()%(max-min+1));
	}

	return 0;
}
