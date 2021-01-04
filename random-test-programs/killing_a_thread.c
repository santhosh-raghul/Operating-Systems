#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

pthread_t test_thread;

void *thread_test_run (void *v) // I must not modify this function
{
    int i=1;
    while(1)
    {
       printf("into thread %d\r\n",i);
       i++; 
       sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_create(&test_thread, NULL, &thread_test_run, NULL);

    sleep (5);

	pthread_cancel(test_thread);


    return 0;
}
