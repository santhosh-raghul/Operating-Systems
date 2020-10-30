#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>
#include<sys/sysinfo.h>

typedef struct par
{
	int r,count;
}par;

void* monte_carlo_runner(void* _param);
int randrange(int a, int b);

int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		fprintf(stderr,"%s: invalid usage\ncorrect syntax: %s range count\nwhere range is the range of random values to choose from and count is the number of simulations\n",argv[0],argv[0]);
		exit(-1);
	}

	int r=atoi(argv[1]),
		n=atoi(argv[2]),
		N=get_nprocs_conf(), // get number of processor cores in N to create N number of threads
		*circle,
		tot_circle=0;

	if(r<=0||n<=0)
	{
		fprintf(stderr,"%s: invalid arguments\ncorrect syntax: %s range count\nwhere range is the range of random values to choose from and count is the number of simulations\n",argv[0],argv[0]);
		exit(-1);
	}

	long double pi;
	par param[N];
	pthread_t tid[N];
	pthread_attr_t attr[N];

	printf("parameters are: %d, %d\n",r,n);
	printf("generating %d random points in the range 0 to %d\n\n",n,r);
	printf("number of CPU cores was found to be %d so creating %d threads\n",N,N);

	for(int i=0;i<N;i++)
	{
		param[i].r=r;
		param[i].count=n/N+(n%N>i?1:0);	// calc number of random points to be generated in thread i
		printf("\tthread %d will generate %d random points\n",i+1,param[i].count);
		pthread_attr_init(&attr[i]);
		pthread_create(&tid[i],&attr[i],monte_carlo_runner,&param[i]);
	}
	printf("generating %d points totally\n\n",n);

	for(int i=0;i<N;i++)
	{
		pthread_join(tid[i],(void*)&circle);	// get number of points inside circle
		tot_circle+=*circle;	// add to total number of points in the circle
		free(circle);
	}
	
	pi=((long double)tot_circle)/n*4;
	printf("out of the %d random points generated, %d were found to be inside the circle\n",n,tot_circle);
	printf("pi value obtained = 4*%d/%d = \e[1m%Lf\e[0m\n",tot_circle,n,pi);
	printf("error = %Lf %%\n",(pi-M_PI)*100);

	return 0;
}

int randrange(int a, int b)
{
	return a+rand()%(b-a+1);
}

void* monte_carlo_runner(void* _param)
{
	par param=*(par*)_param;
	int x,y,r=param.r,count=param.count,*circle=(int*)malloc(4);
	long rsq=r*r;
	srand(time(0));
	// generate count number of points and increment circle for
	// the points which lie inside the circle
	for(int i=0;i<count;i++)
	{
		x=randrange(0,r);
		y=randrange(0,r);
		if(x*x+y*y<=rsq)
			*circle=*circle+1;
	}
	pthread_exit((void*)circle);
}