#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include <stdlib.h>

int main()
{
    pid_t pid;
    int n;
    printf("enter n value : ");
    scanf("%d",&n);
    pid=fork();
    if(pid==-1)
    {
        printf("fork failed\n");
        exit(1);
    }
    else if(pid>0) // parent block
    {
        printf("This is the parent block, odd series : ");
        for(int i=1;i<=n;i+=2)
            printf("%d ",i);
        printf("\n");
    }
    else // child block
    {
        printf("This is the child block, even series : ");
        for(int i=0;i<=n;i+=2)
            printf("%d ",i);
        printf("\n");
    }
    return 0;
}