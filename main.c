#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include "queue.h"

// Function to check if a number is prime
int isprime(int num) {
    // Starts checking from left to right, step by step
    if (num <= 1) return 0;
    if (num <= 3) return 1;
    if (num % 2 == 0 || num % 3 == 0) return 0;
    for (int i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0) return 0;
    }
    return 1;
}

// Function to print first 10 divisors of a number
void printdivisors(int num) {
    printf("Divisors:");
    int divisor_count = 0;  // Counter for the number of divisors printed
    for (int i = 1; i <= num; i++) {
        if (num % i == 0) {
            printf(" %d", i);
            divisor_count++;
            if (divisor_count >= 10) break;  // Exit loop after printing 10 divisors
        }
    }
    printf("\n");
}

// Generator thread function
void *generatorthread(void *arg) {

    //allowing functions to reach members of queue
    Queue *queue = (Queue *)arg;
    
    //taking required values from queue
    int lowbound = queue->lowbound;
    int upbound = queue->upbound;
    int numnumbers = queue->numnumbers;
    int rate = queue->rate;
    
    //we generate random number, than we insert into queue, finally by using given formula we create sleeptime, and thread sleeps according to this.
    for (int i = 0; i < numnumbers; i++) {
        int value = rand() % (upbound - lowbound + 1) + lowbound;
        QueueInsert(queue, value);
        int sleeptime = (int)((-1.0 / rate) * log((double)rand() / RAND_MAX) * 1000000);
        usleep(sleeptime);
    }
    return NULL; //since we used void
}

// Worker thread function, for taking elements of queue one by one and check if it is prime or not.
void *workerthread(void *arg) {
    Queue *queue = (Queue *)arg; //allowing function to access queue
    while (1) {
        int value = QueueRemove(queue);
        if (isprime(value)) {
            printf("Thread ID: %lu, Number: %d is a prime number.\n", pthread_self(), value);
        } else {
            printf("Thread ID: %lu, Number: %d is not a prime number.", pthread_self(), value);
            printdivisors(value);
        }
    }
    return NULL; //since we used void 
}

int main(int argc, char *argv[]) {
    int numthreads = 3;
    int maxsize = 5;
    int numnumbers = 10;
    int lowbound = 1;
    int upbound = 100;
    int rate = 100;

    // Parse command line arguments
    int opt;
    while ((opt = getopt(argc, argv, "t:q:r:m:n:g:")) != -1) {
        switch (opt) {
            case 't':
                numthreads = atoi(optarg);
                break;
            case 'q':
                maxsize = atoi(optarg);
                break;
            case 'r':
                numnumbers = atoi(optarg);
                break;
            case 'm':
                lowbound = atoi(optarg);
                break;
            case 'n':
                upbound = atoi(optarg);
                break;
            case 'g':
                rate = atoi(optarg);
                break;
            default:
                // provides guidance how to use the program, for format of the command-line options
                fprintf(stderr, "Usage: %s [-t threads] [-q maxsize] [-r numnumbers] [-m lowbound] [-n upbound] [-g rate]\n", argv[0]);
                exit(EXIT_FAILURE); //when error occurs
        }
    }

    // Initializing the queue with given parameters
    Queue queue;
    QueueInitialize(&queue, maxsize, lowbound, upbound, numnumbers, rate);

    // initializing the random number generator, by using time(NULL) we get randomness and unpredictability for random variables
    srand(time(NULL));

    // creating generator thread
    pthread_t generator_tid;
    pthread_create(&generator_tid, NULL, generatorthread, (void *)&queue);

    // creating worker threads
    pthread_t worker_tids[numthreads];
    for (int i = 0; i < numthreads; i++) {
        pthread_create(&worker_tids[i], NULL, workerthread, (void *)&queue);
    }

    // joining generator thread
    pthread_join(generator_tid, NULL);

    // destroying the queue
    QueueDestroy(&queue);

    return 0;
}
