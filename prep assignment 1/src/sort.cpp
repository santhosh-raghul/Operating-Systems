// This code is for q III)
// to sort an array of integers or floating point or characters passed at command  line
// function overloading has been used 
// the sort function works with int, char or float arrays but the same function has been defined thrice
// I've used the same kind of lesserthan, greaterthan functions as in the previous q for asc,desc order

#include<iostream>
#include<stdlib.h>
using namespace std;

void sort(int* arr,int n,int (*cmp)(int,int));
void sort(float* arr,int n,int (*cmp)(float,float));
void sort(char* arr,int n,int (*cmp)(char,char));
int lesserthan(int a,int b);
int greaterthan(int a,int b);
int lesserthan(float a,float b);
int greaterthan(float a,float b);
int lesserthan(char a,char b);
int greaterthan(char a,char b);

int main(int argc, char *argv[])
{
    char errormsg[]="invalid input\ncorrect usage: ./sort <n> <type> <q> <arr>\nwhere\nn: number of elements in array\ntype: i for integers, f for floating point numbers and c for characters\nq: 1 for ascending order and 0 for descending order\narr: n space seperated integers,floating point numbers or charcters\n";

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
    for(int i=0;i<n;i++)
        printf("%d ",arr[i]);
}

void sort(float* arr,int n,int (*cmp)(float,float))
{
    float temp;
    for(int i=0;i<n;i++)
        for(int j=0;j<(n-i-1);j++)
            if(cmp(arr[j],arr[j+1]))
            {
                temp=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=temp;
            }
    for(int i=0;i<n;i++)
        printf("%f ",arr[i]);
}

void sort(char* arr,int n,int (*cmp)(char,char))
{
    char temp;
    for(int i=0;i<n;i++)
        for(int j=0;j<(n-i-1);j++)
            if(cmp(arr[j],arr[j+1]))
            {
                temp=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=temp;
            }
    for(int i=0;i<n;i++)
        printf("%c ",arr[i]);
}

int lesserthan(int a,int b)
{
    if((b-a)>0) return 1;
    return 0;
}

int greaterthan(int a,int b)
{ 
    if((a-b)>0) return 1;
    return 0;
}

int lesserthan(float a,float b)
{
    if((b-a)>0) return 1;
    return 0;
}

int greaterthan(float a,float b)
{ 
    if((a-b)>0) return 1;
    return 0;
}

int lesserthan(char a,char b)
{
    if((b-a)>0) return 1;
    return 0;
}

int greaterthan(char a,char b)
{ 
    if((a-b)>0) return 1;
    return 0;
}