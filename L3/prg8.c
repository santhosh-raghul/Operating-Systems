#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include <stdlib.h>

int main()
{
    pid_t pid;
    pid=fork();
    if(pid==-1)
    {
        printf("fork failed\n");
        exit(1);
    }
    else if(pid>0) // parent block
    {

    }
    else // child block
    {
        
    }
    return 0;
}