#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include <stdlib.h>

void sort(int a[],int start,int end);
int bin_search_first_occ(int a[], int x, int left, int right);
void bin_search(int a[], int x, int start, int end, int *pos);

int main()
{
    pid_t pid;
    int n,x,count;
    printf("Implementation of binary search.\nEnter no of elements : ");
    scanf("%d",&n);
    printf("Enter elements one by one : ");
    int arr[n];
    for(int i=0;i<n;i++)
        scanf("%d",&arr[i]);
    printf("Enter element to be searched : ");
    scanf("%d",&x);

    pid=vfork();
    if(pid==-1)
    {
        printf("fork failed\n");
        exit(1);
    }
    else if(pid>0) // parent block
    {
        printf("\nThis is parent block. Searching in first half is done here.\n");
        sort(arr,0,n/2-1);
        printf("Sorting in 1st half completed! Now starting search.\n");
        int pos[2];
        bin_search(arr,x,0,n/2-1,pos);
        if(*pos==-1)
            printf("%d was not found in the 1st half.\n",x);
        else if(*pos==*(pos+1))
            printf("%d occurs once in the 1st half, at index %d.\n",x,*pos);
        else
            printf("%d occurs %d times in the 1st half, from index %d to %d.\n",x,*(pos+1)-*pos+1,*pos,*(pos+1));
    }
    else // child block
    {
        printf("\nThis is child block. Searching in second half is done here.\n");
        sort(arr,n/2,n-1);
        printf("Sorting in 2nd half completed! Now starting search.\n");
        int pos[2];
        bin_search(arr,x,n/2,n-1,pos);
        if(*pos==-1)
            printf("%d was not found in the 2nd half.\n",x);
        else if(*pos==*(pos+1))
            printf("%d occurs once in the 2nd half, at index %d (realtive index %d).\n",x,*pos,*pos-n/2);
        else
            printf("%d occurs %d times in the 2nd half, from index %d to %d.\n",x,*(pos+1)-*pos+1,*pos,*(pos+1));
        exit(0);
    }
    printf("\nArray after sorting 2 halves separately: ");
    for(int i=0;i<n;i++)
        printf("%d ",arr[i]);
    printf("\n");
    return 0;
}

void sort(int a[],int start,int end)
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

void bin_search(int a[], int x, int start, int end,int* pos)
{
    // pos={-1,-1};
    *pos=bin_search_first_occ(a,x,start,end);
    if(pos[0]!=-1)
    {
        pos[1]=pos[0];
        while(a[pos[1]]==x)
            pos[1]++;
        pos[1]--;
    }
}

int bin_search_first_occ(int a[], int x, int left, int right)
{
    if(left==right)
        return left;

    int mid = (right+left)/2;

    if(a[mid]<x) // right half
        return bin_search_first_occ(a, x, mid+1, right);
    else // left half
        return bin_search_first_occ(a, x, left, mid);
}