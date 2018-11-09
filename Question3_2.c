#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

// Global variables
long thread_count;
long long n;
double sum;
pthread_mutex_t mutex;

void *Thread_sum(void *rank);

int main(int argc, char *argv[]) {
    long thread;    // Use long in case of 64-bit system
    pthread_t *thread_handles;

    n = 8192;
    thread_count = 16;

    thread_handles = (pthread_t *)malloc(thread_count * sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);
    sum = 0.0;

    // Create threads
    for (thread = 0; thread < thread_count; thread++) {
        pthread_create(&thread_handles[thread], NULL, Thread_sum, (void *)thread);
    }

    // Join threads
    for (thread = 0; thread < thread_count; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }

    sum = -1.0 * sum;
    printf("With n = %lld terms,\n", n);
    printf("    Our estimate of value = %.15f\n", sum);

    pthread_mutex_destroy(&mutex);
    free(thread_handles);
    return 0;
}

void *Thread_sum(void *rank) {
    long my_rank = (long)rank;
    long long i;
    long long my_n = n / thread_count;
    long long my_first_i = my_n * my_rank + 1;
    long long my_last_i = my_first_i + my_n + 1;
    double my_sum = 0.0;

    for (i = my_first_i; i < my_last_i; i++) {
        my_sum += pow(0.5, (double) i)/((double) i);
    }

    pthread_mutex_lock(&mutex);
    sum += my_sum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}