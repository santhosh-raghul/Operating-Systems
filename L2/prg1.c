#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main()
{
    // pid_t pid;
    // pid=fork();
    // if (pid!=0)
    //     fork();
    // fork();
    // printf("Count \n");
    // return 0;
    printf("This is process %d with parent %d\n",getpid(),getppid());
}