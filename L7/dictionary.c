#include<stdio.h>
#include<sys/wait.h>
#include<pthread.h>
#include<string.h>

#define BUFFER_SIZE 5
#define FULL (in+1)%BUFFER_SIZE==out
#define EMPTY in==out
#define WORD_SIZE 16
#define MEANING_SIZE 128
#define MAX 10
#define PRODUCER 0
#define CONSUMER 1
#define FALSE 0
#define TRUE 1

struct dict
{
	char word[WORD_SIZE],meaning_1[MEANING_SIZE],meaning_2[MEANING_SIZE];
};

void* producer(void* par);
void* consumer(void* par);

int in=0,out=0,turn=0,flag[2]={0,0},end=FALSE,found=FALSE;
struct dict buf[BUFFER_SIZE];
char search_key[30];

int main(int argc,char* argv[])
{
	if(argc!=2)
	{
		fprintf(stderr,"invalid usage!\ncorrect usage: %s word\n",argv[0]);
		return 1;
	}
	printf("note: actual order of execution may vary from the order of printf\n");
	pthread_t tid[2];
	strcpy(search_key,argv[1]);
	pthread_create(&tid[0],NULL,producer,NULL);
	pthread_create(&tid[1],NULL,consumer,NULL);
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	if(!found)
	{
		fprintf(stderr,"\033[1;31mword not found\n");
		return 1;
	}
	return 0;
}

void* producer(void* par)
{
	char ch;
	int k,index;
	FILE* dictionary_file;
	if((dictionary_file=fopen("words.csv","r"))==NULL)
		perror("file not found");
	while(TRUE)
	{
	// wait while full
		while(FULL);

	// petersons solution lock for mutual exclusion
		flag[PRODUCER]=TRUE;
		turn=CONSUMER;
		while(flag[CONSUMER] && turn==CONSUMER);

	// critical section - produce data in this case
		// get word
		k=0;
		while((ch=fgetc(dictionary_file))!=',')
			buf[in].word[k++]=ch;
		buf[in].word[k]='\0';

		// get meaning 1
		k=0;
		while((ch=fgetc(dictionary_file))!=',')
			buf[in].meaning_1[k++]=ch;
		buf[in].meaning_1[k]='\0';

		// get meaning 2
		k=0;
		while((ch=fgetc(dictionary_file))!=EOF)
		{
			if(ch=='\n')
				break;
			buf[in].meaning_2[k++]=ch;
		}
		buf[in].meaning_2[k]='\0';
		index=in;
		in=(in+1)%BUFFER_SIZE;

	// unlock
		flag[PRODUCER]=FALSE;

	// remainder section
		printf("produced \033[1;36m'%s'\033[0m at index %2d\n",buf[index].word,index);
		if(ch==EOF || found)
			break;
	}

	end=TRUE;
	pthread_exit(0);
}

void* consumer(void* par)
{
	struct dict temp;
	int index;
	while(TRUE)
	{
	// wait while empty
		while(EMPTY && !end);
	// petersons solution lock for mutual exclusion
		flag[CONSUMER]=TRUE;
		turn=PRODUCER;
		while(flag[PRODUCER] && turn==PRODUCER);
	// critical section - consume data in this case
		temp=buf[out];
		index=out;
		out=(out+1)%BUFFER_SIZE;
	// unlock
		flag[CONSUMER]=FALSE;
	// remainder section
		printf("consumed \033[1;35m'%s'\033[0m at index %2d\n",temp.word,index);
		if(strcmp(temp.word,search_key)==0)
		{
			printf("word \033[1;33m'%s'\033[0m found\n      \033[1m├──── %s\n      └──── %s\n\033[0m",temp.word,temp.meaning_1,temp.meaning_2);
			found=TRUE;
		}
		if(found || (EMPTY && end)) // if the word has been found or all the words have been consumed
			break; // exit
	}
	pthread_exit(0);
}