// This code is for q I) ii)
// to simulate the behavior of rm command in linux

#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
    if(argc<=1)
    {
        printf("missing operand\n");
        exit(-1);
    }
    for(int i=1;i<argc;i++)
    {
        if(remove(argv[i])==0)
            printf("%s deleted successfully\n",argv[i]);
        else
            printf("%s was not found / could not be deleted\n",argv[i]);
    }
    return 0;
}