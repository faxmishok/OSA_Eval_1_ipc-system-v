#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

int thread_num=0;
pthread_mutex_t lock;

void *counter(void *argp) {
   

    int counter = 0;
    
    int *thread_number = (int *) argp;
    printf(" My ThreadID is %d \n",*thread_number);
    pthread_mutex_lock(&lock);
    
    int mynum=++thread_num;
    
    pthread_mutex_unlock(&lock);
    
    struct timespec ts;
    srand(time(NULL));
    
    while (counter < 10) {
        ts.tv_nsec = (((double)rand()) / RAND_MAX) * 1000000000;
        nanosleep(&ts, NULL);
        printf("After %ld nanoseconds of sleep %dth thread at counter %d\n",ts.tv_nsec, *thread_number , counter);
        counter++;
    }
    printf("I am thread number %d and i am done\n",mynum);
    return NULL;
}

int main () {
    
    int n;
    printf("Give a number of threads to be created\n");
    scanf("%d", &n);

    pthread_t thread_id[n];
    for (int i = 0; i < n; i++) {
         pthread_create(&thread_id[i], NULL, counter, &thread_id[i]);
    }
    for (int i = 0; i < n; i++) {
        pthread_join(thread_id[i], NULL);

    }
    return 0; 
}