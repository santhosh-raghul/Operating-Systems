#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

int M[1024][1024];

typedef struct param
{
	char *s1,*s2;
	int l1,l2;
}param;

void* lcs_dp_runner(void* _param);
void* lcs_construct_runner(void* _param);
char* longest_common_subsequence(char* s1,char* s2);

int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		fprintf(stderr,"%s: invalid usage\ncorrect syntax: %s string1 string2\nto display the longest common subsequence of string1 and string2\n",argv[0],argv[0]);
		exit(-1);
	}

	printf("'%s'\n",longest_common_subsequence(argv[1],argv[2]));

	return 0;
}

char* longest_common_subsequence(char* s1,char* s2)
{
	char *lcs;

	pthread_t tid1,tid2;
	pthread_attr_t attr1,attr2;
	pthread_attr_init(&attr1);
	pthread_attr_init(&attr2);

	param par;
	par.s1=s1;
	par.s2=s2;
	par.l1=strlen(s1);
	par.l2=strlen(s2);

	pthread_create(&tid1,&attr1,lcs_dp_runner,&par);	// find M[i][j] (dynamic programming)
	pthread_join(tid1,NULL);
	pthread_create(&tid2,&attr2,lcs_construct_runner,&par);	// find lcs from M[][]
	pthread_join(tid2,(void*)&lcs);

	return lcs;
}

void* lcs_dp_runner(void* _param)
{
	param par=*(param*)_param;
	for(int i=0;i<=par.l1;i++)
	{
		for(int j=0;j<=par.l2;j++)
		{
			if(i==0||j==0)
				M[i][j]=0;
			else if(par.s1[i-1]==par.s2[j-1])
				M[i][j]=M[i-1][j-1]+1;
			else
				M[i][j]=(M[i-1][j]>M[i][j-1])?M[i-1][j]:M[i][j-1];
		}
	}
	pthread_exit(0);
}

void* lcs_construct_runner(void* _param)
{
	param par=*(param*)_param;
	int current,i=par.l1,j=par.l2;
	current=M[i][j];
	char *lcs=(char*)malloc(current+1);
	lcs[current--]='\0'; 
	while(i>0&&j>0) 
	{
		if(par.s1[i-1]==par.s2[j-1]) 
		{
			lcs[current--]=par.s1[i-1];
			i--;
			j--;
		}
		else if (M[i-1][j]>M[i][j-1]) 
			i--;
		else
			j--; 
	}
	pthread_exit((void*)lcs);
}