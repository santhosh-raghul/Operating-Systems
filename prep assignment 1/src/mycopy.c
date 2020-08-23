// This code is for q I) i)
// to simulate the behavior of cp command in linux

#include<stdio.h>
#include<stdlib.h>

int main(int argc, char* argv[])
{
	if(argc==1)
    {
        printf("missing file operand\n");
		printf("correct usage: ./cp <path-to-source-file> <path-to-destination-file>\n");
        return -1;
    }
    else if(argc==2)
    {
		printf("missing destination file operand after '%s'\n",argv[1]);
		printf("correct usage: ./cp <path-to-source-file> <path-to-destination-file>\n");
		return -1;
	}
    else if(argc!=3)
    {
        printf("wrong usage of cp\n");
		printf("correct usage: ./cp <path-to-source-file> <path-to-destination-file>\n");
        return -1;
    }

	FILE *source, *dest;
	char *sourcefile=argv[1];
	char *destinationfile=argv[2];

	source=fopen(sourcefile,"rb");
	if(source==NULL)
	{
		printf("cannot stat '%s': No such file or directory\n",argv[1]);
		printf("correct usage: ./cp <path-to-source-file> <path-to-destination-file>\n");
        return -1;
	}
	dest=fopen(destinationfile,"wb");

	int c;
	while((c=fgetc(source))!=EOF)
    {
		fputc(c, dest);
	}
	fclose(source);
	fclose(dest);

	printf("copied file successfully\n");
	return 0;
}