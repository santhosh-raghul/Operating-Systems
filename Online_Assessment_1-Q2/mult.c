#include<stdio.h>
#include<stdlib.h>

int main(int argc,char *argv[])
{
	if(argc!=3)
		exit(-1);
	
	// get i, j values
	int i=atoi(argv[1]),j=atoi(argv[2]),sum=0,a,b,n;

	// open files
	FILE *af,*bf;
    af=fopen("a.mat","r");
    bf=fopen("b.mat","r");
	// get n value
	fscanf(af,"%d",&n);
	fscanf(bf,"%d",&n);

	// go to the place where the data has to be read from
	// (i.e) i th row in a and
	// j th row in b (row because transpose of b is stored)
	// so j th row of transpose will actually be j th column
	for(int k=0;k<i;k++)
		for(int l=0;l<n;l++)
		fscanf(af,"%d",&a);
	for(int k=0;k<j;k++)
		for(int l=0;l<n;l++)
		fscanf(bf,"%d",&a);

	for(int k=0;k<n;k++)
	{
		// read values
		fscanf(af,"%d",&a);
		fscanf(bf,"%d",&b);
		// save the value in sum
		sum+=a*b;
	}
	
	// close both files
	fclose(af);
	fclose(bf);

	// put sum in ans.mat and close it
	af=fopen("ans.mat","a");
    fprintf(af,"%d %d %d\n",i,j,sum);
	fclose(af);

	// exit the program
	exit(0);
}