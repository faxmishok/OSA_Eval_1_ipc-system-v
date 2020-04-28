#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

int fakeIdThread = 0; /* global variable independant from how many times function is called.
this is to determine how many threads survived and which one is which */

pthread_mutex_t lock;

void *function(void *arg) { //a pointer function for demands of pthread_create() function

    pthread_mutex_lock(&lock);      //to intervene another thread get past this line, if it does then fakeIdThread numbers will get mixed
    
    int whichThread = ++fakeIdThread; //since thread_id's can be complicated to figure out while printing, 
    //each time a thread is entering here will be assigned a new unique number which is incremented 1 by 1
    
    pthread_mutex_unlock(&lock);    // once its whichThread is set, we can unlock 
    
    struct timespec ts;
    srand(time(NULL)); // to get completely random numbers from rand() each time.If we don't bind it to time() rand() will return the same numbers 
    
    for (int i = 0 ; i < 10 ; i++ ) {
       
        ts.tv_nsec = (((double)rand()) / RAND_MAX) * 1000000000;  // rand returns integer therefore casting to double to successfully divide into RAND_MAX
        // and why divide into RAND_MAX is because the largest int it can return is no more than RAND_MAX
        // so dividing into it and multiplying by 10 to the power of 9 will give the exact amount of zeros to represent nanoseconds 
        nanosleep(&ts, NULL);
        printf("Thread #%d <---> ",(int*)arg); // must cast to print as int
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
    
    if (pthread_mutex_init(&lock, NULL) != 0) { 
        printf("\n mutex init has failed\n"); 
        return 1; 
    }
    
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