#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

int fakeIdThread = 0; /* global variable independant from how many times function is called.
this is to determine how many threads survived and which one is which */

pthread_mutex_t lock;

void *function(pthread_t *th) { //a pointer function for demands of pthread_create() function

    printf(" My ThreadID is %d \n",(int*)th);

    pthread_mutex_lock(&lock);      //to intervene another thread get past this line, if it does then fakeIdThread numbers will get mixed
    
    int whichThread = ++fakeIdThread;
    
    pthread_mutex_unlock(&lock);    // once mynum is set, we can unlock 
    
    struct timespec ts;
    srand(time(NULL)); // to get completely random numbers from rand() each time.If we don't bind it to time() rand() will return the same numbers 
    
    for (int i = 0 ; i < 10 ; i++ ) {
       
        ts.tv_nsec = (((double)rand()) / RAND_MAX) * 1000000000;
        nanosleep(&ts, NULL);
        printf("Thread #%d <---> ",(int*)th); // must cast (pthread_t *) to (int *)
        printf("slept for %ld nanoseconds and ",ts.tv_nsec);
        printf("is on count number %d\n",i);
    }

    switch (whichThread)      // just for fancy printing.
    {
    case 1:
        printf("%dst thread is finished.\n",whichThread);
        break;
    case 2:
        printf("%dnd thread is finished.\n",whichThread);
        break;
    case 3:
        printf("%drd thread is finished.\n",whichThread);
        break;
    default:
        printf("%dth thread is finished.\n",whichThread);
        break;
    }


    return NULL;
}

int main (int argc, char *argv[]) {
    
    int nThread;
    printf("Enter the number of threads you want to generate: ");
    scanf("%d",&nThread);

    pthread_t threads[nThread]; //array of pthread_t to store threads and do further operations
    
    for (int i = 0 ; i < nThread ; i++) {
        pthread_create(&threads[i], NULL, function, &threads[i]);
    }   //function ptr is passed because pthread_create() will invoke conditionally. &threads[i] is the 1st arg passed inside function()

    for (int i = 0 ; i < nThread ; i++) {
        pthread_join(threads[i], NULL);
    }
    

    return 0; 
}