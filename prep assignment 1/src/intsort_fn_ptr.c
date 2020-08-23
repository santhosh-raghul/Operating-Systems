// This code is for q II) Extra Credits Qn
// to sort an array of varying number of integers in ascending or descending order
// Extra Credits Qn was to implement the same using only function internally using function pointers
// I have declared and defined 2 functions greaterthan and lessthan which will be used inside the sort function
// greaterthan will return 1 when a>b and lessthan will return 1 when a<b
// I have implemented bubble sort in the sort function but the comparison takes place using fn pointers
// If ascending order is needed, greaterthan is passed as a function pointer for comparisons to take place
// If descending order is needed, lessthan is passed as a function pointer for comparisons to take place
// so, the final output will be whatever order is required - ascending or descending

#include<stdio.h>
#include<stdlib.h>

void sort(int* arr,int n,int (*cmp)(int,int));
int lessthan(int a,int b);
int greaterthan(int a,int b);

int main(int argc, char *argv[])
{
    char errormsg[]="invalid input\ncorrect usage: ./intsort_fn_ptr <n> <q> <arr>\nwhere\nn: number of elements in array\nq: 1 for ascending order and 0 for descending order\narr: n space seperated integers\n";

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
        sort(arr,n,greaterthan);
    else if(atoi(argv[2])==0)
        sort(arr,n,lessthan);
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

void sort(int* arr,int n,int (*cmp)(int,int))
{
    int temp;
    for(int i=0;i<n;i++)
        for(int j=0;j<(n-i-1);j++)
            if(cmp(arr[j],arr[j+1]))
            {
                temp=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=temp;
            }
}

int lessthan(int a,int b)
{
    if((b-a)>0) return 1;
    return 0;
}

int greaterthan(int a,int b)
{ 
    if((a-b)>0) return 1;
    return 0;
}