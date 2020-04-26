#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long int nGlobal = 0;

void *increment(pthread_t *th) {

    printf("Thread number %d is doing its job now...\n",(int*)th);
    for ( int i = 0 ; i < 1000000 ; i++ ) nGlobal++;
    printf("Thread number %d finished incrementing and value of the counter is: %d\n",(int*)th, nGlobal);

    return NULL;
}

int main() {

    pthread_t threads[4];

    for ( int i = 0 ; i < 4 ; i++ ) {
        pthread_create(&threads[i], NULL, increment, &threads[i]);
    }

    for ( int i = 0 ; i < 4 ; i++ ) {
        pthread_join(threads[i], NULL);
    }

    printf("Global value at the end is : %d\n",nGlobal);

    return 0;
}