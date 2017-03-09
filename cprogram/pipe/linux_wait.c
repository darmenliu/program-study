#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

pthread_t       tid1, tid2; 
void            *tret;

pthread_mutex_t clifd_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t clifd_cond = PTHREAD_COND_INITIALIZER;


void *
thr_fn1(void *arg)
{
    printf("thread 1 starting\n");
    sleep(100);
    printf("thread 2 exit code %d\n", (int)tret);
    printf("thread 1 returning\n");
    return((void *)2);
}


int
main(void)
{
    int            err;
    int wait_status;

    err = pthread_create(&tid1, NULL, thr_fn1, NULL);
    if (err != 0)
        printf("can't create thread 1\n");

    printf("thread 1 exit code %d\n", (int)tret);
    
//    wait(NULL);
    pthread_cond_wait(&clifd_cond, &clifd_mutex);
    exit(0);
}

