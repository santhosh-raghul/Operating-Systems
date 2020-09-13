#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include <stdlib.h>

int main()
{
    pid_t pid;
    int n,sum=0;
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
        printf("This is the parent block, sum of odd terms from 0 to %d = ",n);
        for(int i=1;i<=n;i+=2)
            sum+=i;
        printf("%d\n",sum);
    }
    else // child block
    {
        printf("This is the child block, sum of even terms from 0 to %d = ",n);
        for(int i=0;i<=n;i+=2)
            sum+=i;
        printf("%d\n",sum);
    }
    return 0;
}