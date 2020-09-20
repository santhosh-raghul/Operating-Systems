#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include <stdlib.h>

void asc_sort(int a[],int start,int end);
void desc_sort(int a[],int start,int end);

int main()
{
    pid_t pid;
    int n;
    printf("Enter no of elements : ");
    scanf("%d",&n);
    printf("Enter elements one by one : ");
    int arr[n];
    for(int i=0;i<n;i++)
        scanf("%d",&arr[i]);

    pid=vfork();
    if(pid==-1)
    {
        printf("fork failed\n");
        exit(1);
    }
    else if(pid>0) // parent block
    {
        printf("This is parent block. Sorting first half in ascending order is done here\n");
        asc_sort(arr,0,n/2-1);
    }
    else // child block
    {
        printf("This is child block. Sorting second half in descending order is done here\n");
        desc_sort(arr,n/2,n-1);
        exit(0);
    }
    printf("Array after sorting : ");
    for(int i=0;i<n;i++)
        printf("%d ",arr[i]);
    printf("\n");
    return 0;
}

void asc_sort(int a[],int start,int end)
{
    int temp;
    for(int i=start;i<=end;i++)
        for(int j=start;j<(end-i+start);j++)
            if(a[j]>a[j+1])
            {
                temp=a[j];
                a[j]=a[j+1];
                a[j+1]=temp;
            }
}

void desc_sort(int a[],int start,int end)
{
    int temp;
    for(int i=start;i<=end;i++)
        for(int j=start;j<(end-i+start);j++)
            if(a[j]<a[j+1])
            {
                temp=a[j];
                a[j]=a[j+1];
                a[j+1]=temp;
            }
}