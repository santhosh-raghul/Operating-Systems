#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

int main()
{
    int x=0;
    pid_t pid1,pid2;
    pid1=fork(); // 1 forks 2 here
    if(pid1==-1)
        printf("fork failed\n");
    else if(pid1>0) // parent block
    {
        pid2=fork(); // 1 forks 3 here
        if(pid2==-1)
            printf("fork failed");
        else if(pid2==0) // inner child block
            fork(); // 3 forks 7 here
    }
    else //child block
    {
        pid2=fork(); // 2 forks 4 here
        if(pid2==-1)
            printf("fork failed");
        else if(pid2>0) // inner parent block
        {
            fork(); // 2 forks 5 here
            fork(); // 2 forks 6 and 5 forks 9 here
            // the above fork can also be split as 1 fork() in parent block and another fork() in child block
        }
        else // inner child block
        {
            fork(); // 4 forks 8
        }
        
    }
    while((x=wait(NULL))!=-1); // to make sure all children terminate before the parent to get correct pid, ppid
    printf("This is process %d with parent %d\n",getpid(),getppid());
    return 0;
}
