#include<stdio.h>

int a=0;

void main()
{
	printf("Welcome To %d \n",a);
	a++;
	if (a < 3)
		main();
	printf("OS 2020 %d \n",a);
}