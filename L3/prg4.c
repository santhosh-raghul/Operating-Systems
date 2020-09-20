#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<math.h>

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
        int x=0, y=1, temp;
        printf("This is parent block. Fibonacci sereis generation is done here\n");
        for (int i=1;i<=n;i++)
        {
            printf("%d ",x);
            temp=x+y;
            x=y;
            y=temp;
        }
        printf("\n");
    }
    else // child block
    {
        int x=3,count=1,flag,i;
        printf("This is child block. Prime sereis generation is done here\n");
        if(n>0)
            printf("2 ");
        while(count<n)
        {
            flag=1;
            for(i=2;i<=sqrt(x);i++)
                if(x%i==0)
                { 
                    flag = 0; 
                    break; 
                }
            if(flag)
            {
                printf("%d ",x);
                count++;
            }
            x++;
        }      
        printf("\n");
    }
    return 0;
}