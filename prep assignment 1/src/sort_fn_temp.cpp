// This code is for q IV)
// to sort an array of integers or floating point or characters passed at command  line
// function templates has been used - sort has been defined only once
// the sort function works with not only int, char or float arrays but more though sort has been defined only once
// I've used the same kind of lesserthan, greaterthan functions as in the previous q for asc,desc order
// but this time, I've used function templates even for lesserthan and greaterthan functions

#include<iostream>
#include<stdlib.h>
using namespace std;

template<typename t>
int greaterthan(t a,t b)
{
    int x=a>b;
    return x;
}

template<typename t>
int lesserthan(t a,t b)
{
    int x=a<b;
    return x;
}

template<typename t>
void sort(t* arr,int n,int (*cmp)(t,t))
{
    t temp;
    for(int i=0;i<n;i++)
        for(int j=0;j<(n-i-1);j++)
            if(cmp(arr[j],arr[j+1]))
            {
                temp=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=temp;
            }
    for(int i=0;i<n;i++)
        cout<<arr[i]<<" ";
}

int main(int argc, char *argv[])
{
    char errormsg[]="invalid input\ncorrect usage: ./sort_fn_temp <n> <type> <q> <arr>\nwhere\nn: number of elements in array\ntype: i for integers, f for floating point numbers and c for characters\nq: 1 for ascending order and 0 for descending order\narr: n space seperated integers,floating point numbers or charcters\n";

    if(argc<5)
    {
        printf("%s",errormsg);
        exit(-1);
    }

    int n=atoi(argv[1]);
    char type=argv[2][0];
    int order=atoi(argv[3]);

    if(n!=(argc-4)||n==0||(order!=1 && order!=0))
    {
        printf("%s",errormsg);
        exit(-1);
    }

    if(type=='i')
    {
        int arr[n];
        for(int i=0;i<n;i++)
            arr[i]=atoi(argv[i+4]);
        if(order)
            sort(arr,n,greaterthan);
        else
            sort(arr,n,lesserthan);
    }
    else if(type=='f')
    {
        float arr[n];
        for(int i=0;i<n;i++)
            arr[i]=atof(argv[i+4]);
        if(order)
            sort(arr,n,greaterthan);
        else
            sort(arr,n,lesserthan);
    }
    else if(type=='c')
    {
        char arr[n];
        for(int i=0;i<n;i++)
            arr[i]=argv[i+4][0];
        if(order)
            sort(arr,n,greaterthan);
        else
            sort(arr,n,lesserthan);
    }
    else
    {
        printf("%s",errormsg);
        exit(-1);
    }

    
    printf("\n");
    return 0;
}