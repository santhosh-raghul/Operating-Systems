// all the sleep() calls have been written just for readability and for randomness in the spectator thread

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<pthread.h>
#include<semaphore.h>

#define FALSE 0
#define TRUE 1
#define NO_IMMIGRANTS 8 // macro to define number of immigrants threads
#define NO_SPECTATORS 2 // macro to define number of spectators threads

// judge thread's runner function
void* judge_runner(void* param);
// immigrant thread's runner function
void* immigrant_runner(void* param);
// spectator thread's runner function
void* spectator_runner(void* param);
// function for judge entering the building
void judge_enter();
// function for judge confirming the certificate of one of the immigrants
void judge_confirm();
// function for judge leaving the building
void judge_leave();
// function for immigrant entering the building 
void immigrant_enter();
// function for immigrant checking in 
void immigrant_check_in();
// function for immigrant getting the certificate confirmed by the judge
void immigrant_get_certificate();
// function for immigrant leaving the building
void immigrant_leave();
// function for spectator entering the building
void spectator_enter();
// function for spectator confirming the certificate of one of the immigrants
void spectator_spectate();
// function for spectator leaving the building
void spectator_leave();
// signal handler function to print an exit message and exit out of the program when user presses ^C
void int_sig_handler(int signum);

int new_immigrants=0, // contains the count of immigrants in the building who have not got their certificate yet
	immigrants_checked_in=0, //contains the number of immigrants in the building who have checked in and are yet to get their certificate
	old_immigrants=0, // contains the count of immigrants in the building who have got their certifacte and are about to leave
	judge_in_building=FALSE; // true if judge is in the building, false if not

sem_t immigrants_main_count_mutex, // mutex for keeping count of number of immigrants in the building
	immigrants_checked_in_mutex, // mutex for keeping count of number of immigrants who have checked in 
	certificate_semaphore; // semaphore for synchronizing issual of certificate by the judge and collection by the immigrants

int main()
{
	int i;
	pthread_t judge_thread,spectator_threads[NO_SPECTATORS],immigrant_threads[NO_IMMIGRANTS];

	signal(SIGINT,int_sig_handler);

	// initializing mutex to 1
	sem_init(&immigrants_main_count_mutex,0,1);
	sem_init(&immigrants_checked_in_mutex,0,1);

	// intializing semaphore to 0 - will be posted by the judge and collected by the immigrants
	sem_init(&certificate_semaphore,0,0);
	
	pthread_create(&judge_thread,NULL,judge_runner,NULL);
	for(i=0;i<NO_IMMIGRANTS;i++)
		pthread_create(&immigrant_threads[i],NULL,immigrant_runner,NULL);
	for(i=0;i<NO_SPECTATORS;i++)
		pthread_create(&spectator_threads[i],NULL,spectator_runner,NULL);

	pthread_join(judge_thread,NULL);
	for(i=0;i<NO_IMMIGRANTS;i++)
		pthread_join(immigrant_threads[i],NULL);
	for(i=0;i<NO_SPECTATORS;i++)
		pthread_join(spectator_threads[i],NULL);

	return 0;
}

void* judge_runner(void* param)
{
	while(TRUE)
	{
		while(old_immigrants>0); // judge should not enter when there are immigrants who got their certificate
		judge_enter(); // judge enters the building
		sleep(1);
		while(new_immigrants>immigrants_checked_in); // judge cannot start confirming unless all immigrants entered have checked in
		while(immigrants_checked_in) // while there are immigrants who have not got their certificates yet, confirm immigrants
		{
			sleep(1);
			judge_confirm(); // judge confirms the certificate of one of the immigrants
		}
		judge_leave(); // once all certificates have been confirmed, judge leaves the building
	}
}

void* immigrant_runner(void* param)
{
	while(TRUE)
	{
		while(judge_in_building); // no one enter when judge is there in the building
		immigrant_enter(); // immigrant enters the building
		immigrant_check_in(); // immigrant checks in
		sem_wait(&certificate_semaphore); // wait for judge to confirm certificate
		immigrant_get_certificate(); // immigrant waits and gets certificate
		while(judge_in_building); // immigrant cannot leave when judge is still in the building
		immigrant_leave(); // immigrant leaves the building
	}
}

void* spectator_runner(void* param)
{
	while(TRUE)
	{
		sleep(2+rand()%5);
		while(judge_in_building); // no one enter when judge is there in the building
		spectator_enter(); // spectator enters the building
		spectator_spectate(); // spectator starts spectating
		sleep(10+rand()%5);
		spectator_leave(); // spectator leaves the building (specator can leave anytime)
	}
}

void judge_enter()
{
	judge_in_building=TRUE;
	printf("judge entered\n");
}

void judge_confirm()
{
	sem_post(&certificate_semaphore); // post certificate so that immigrant can collect
	immigrants_checked_in--; // decrement immigrants checked in 
	printf("judge confirmed an immigrant, remaining = %d\n",immigrants_checked_in);
}

void judge_leave()
{
	judge_in_building=FALSE;
	printf("judge left\n");
}

void immigrant_enter()
{
	sem_wait(&immigrants_main_count_mutex); // wait for mutex
	new_immigrants++; // increment new immigrants
	printf("an immigrant entered and is waiting in line, total = %d\n",new_immigrants);
	sem_post(&immigrants_main_count_mutex); // release mutex lock
}

void immigrant_check_in()
{
	sem_wait(&immigrants_checked_in_mutex); // wait for mutex
	immigrants_checked_in++; // increment immigrants checked in
	printf("an immigrant checked in and sat down, total = %d\n",new_immigrants);
	sem_post(&immigrants_checked_in_mutex); // release mutex lock
}

void immigrant_get_certificate()
{
	sem_wait(&immigrants_main_count_mutex); // wait for mutex
	new_immigrants--; // decrement new immigrant
	old_immigrants++; // increment old immigrant
	// we do this because new immigrant now becomes old immigrant after getting certificate
	printf("an immgirant got certificate, remaining = %d\n",immigrants_checked_in);
	sem_post(&immigrants_main_count_mutex); // release mutex lock
}

void immigrant_leave()
{
	sem_wait(&immigrants_main_count_mutex); // wait for mutex
	printf("an immigrant left, remaining = %d\n",--old_immigrants); // decrement old immigrants (note the --)
	sem_post(&immigrants_main_count_mutex); // release mutex lock
}

void spectator_enter()
{
	printf("a spectator entered\n");
}

void spectator_spectate()
{
	printf("a spectator started spectating\n");
}

void spectator_leave()
{
	printf("a spectator left\n");
}

void int_sig_handler(int signum)
{
	printf("\n^C interrupt recieved\nexiting...\n");
	exit(0);
}