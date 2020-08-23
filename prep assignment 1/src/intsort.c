// This code is for q II) i)
// to sort an array of varying number of integers in ascending or descending order

#include<stdio.h>
#include<stdlib.h>

void sort_asc(int* arr,int n);
void sort_desc(int* arr,int n);

int main(int argc, char *argv[])
{
    char errormsg[]="invalid input\ncorrect usage: ./intsort <n> <q> <arr>\nwhere\nn: number of elements in array\nq: 1 for ascending order and 0 for descending order\narr: n space seperated integers\n";

    if(argc<4)
    {
        printf("%s",errormsg);
        exit(-1);
    }

    int n=atoi(argv[1]);

    if(n!=(argc-3)||n==0)
    {
        printf("%s",errormsg);
        exit(-1);
    }

    int *arr=malloc(n*sizeof(int));
    for(int i=0;i<n;i++)
		arr[i] = atoi(argv[i+3]);
    
    if(atoi(argv[2])==1)
        sort_asc(arr,n);
    else if(atoi(argv[2])==0)
        sort_desc(arr,n);
    else
    {
        printf("%s",errormsg);
        exit(-1);
    }

    for(int i=0;i<n;i++)
        printf("%d ",arr[i]);
    printf("\n");
    return 0;
}

void sort_asc(int* arr,int n)
{
    int temp;
    for(int i=0;i<n;i++)
        for(int j=0;j<(n-i-1);j++)
            if(arr[j]>arr[j+1])
            {
                temp=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=temp;
            }
}

void sort_desc(int* arr,int n)
{
    int temp;
    for(int i=0;i<n;i++)
        for(int j=0;j<(n-i-1);j++)
            if(arr[j]<arr[j+1])
            {
                temp=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=temp;
            }
}