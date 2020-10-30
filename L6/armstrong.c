#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void* armstrong_runner(void* num);
int power(int x, unsigned int y);
int order(int a);

int result;

int main(int argc,char* argv[])
{
	if(argc!=2)
	{
		printf("invalid usage\ncorrect usage: %s number\nwhere number is the number until which armstrong numbers are to be printed\n",argv[0]);
		exit(1);
	}
	int n=atoi(argv[1]),sum,num;
	printf("Sum is calculated in the thread and checking if sum == number is done in the main process.\n");
	printf("Armstrong numbers from 1 to %d are :\n",n);
	for(int num=1;num<=n;num++)
	{
		pthread_t tid;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tid,&attr,armstrong_runner,&num); // calculate sum and store in result
		pthread_join(tid,NULL);
		if(result==num) // print the number if sum == number
			printf("%d\n",num);
	}
	return 0;
}

void* armstrong_runner(void* num) // saves the sum in the global variable result
{
	int rem,n,number,*temp;
	temp=(int*)num;
	number=*temp;
	n=order(number);
	result=0;
	while (number)
	{
		int rem = number % 10;
		result += power(rem, n);
		number = number / 10;
	}
	pthread_exit(0);
}

int power(int x, unsigned int y)  // returns x^y
{
	if (y == 0)
		return 1;
	else if (y % 2)
		return  x * power(x, y / 2) * power(x, y / 2);
	return power(x, y / 2) * power(x, y / 2);
}

int order(int a) // return number of digits in a
{
	int n = 0;
	while (a)
	{
		n++;
		a=a/10;
	}
	return n;
}