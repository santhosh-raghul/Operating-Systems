#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>

int armstrong(int x);
int power(int x, unsigned int y);
int order(int a);

int main()
{
    pid_t pid;
    int n,result ,num;
    printf("An Armstrong number is a number that is the sum of its own digits each raised to the power of the number of digits.\nEnter a number n to display all the armstrong numbers until n : ");
    scanf("%d",&n);
    printf("Sum is calculated in the child block and checking if sum == number is done in the parent block.\n");
    printf("Armstrong numbers from 1 to %d are :\n",n);
    for(int num=1;num<=n;num++)
    {
        result =0;
        pid=vfork(); 
        if(pid == -1)
        {
            printf("fork failed \n");
            exit(1);
        }
        else if(pid>0) // parent block
        {
            if(result==num) // print the number if sum == number
                printf("%d\n",num);
        }
        else // child block
        {
            result = armstrong(num); // calculate sum and store in result
            exit(0);
        }
    }
    return 0;
}

int armstrong(int x)
{
    int rem;
    int n = order(x);
    int temp = x, sum = 0;
    while (temp)
    {
        int rem = temp % 10;
        sum += power(rem, n);
        temp = temp / 10;
    }
    return sum;
}

int power(int x, unsigned int y)
{
    if (y == 0)
        return 1;
    else if (y % 2)
        return  x * power(x, y / 2) * power(x, y / 2);
    return power(x, y / 2) * power(x, y / 2);
}

int order(int a)
{
    int n = 0;
    while (a)
    {
        n++;
        a=a/10;
    }
    return n;
}