#include <stdlib.h>
#include <pthread.h>
#include "queue.h"

void QueueInitialize(Queue *queue, int maxsize, int lower_bound, int upper_bound, int num_numbers, int generation_rate) {
    queue->array = (int *)malloc(maxsize * sizeof(int)); // this is for dynamically allocating memory, sizeof is used to be hold enough memory for integers since malloc requires the number of bytes to allocate instead of number of elements.
    // Initializing
    queue->maxsize = maxsize;
    queue->currentsize = 0;
    queue->front = 0;
    queue->rear = -1;
    queue->lowbound = lower_bound;     
    queue->upbound = upper_bound;      
    queue->numnumbers = num_numbers;   
    queue->rate = generation_rate;     
    
    //these lines initialize  mutex and two condition variables with nulls
    pthread_mutex_init(&(queue->mutex), NULL);
    pthread_cond_init(&(queue->notfull), NULL);
    pthread_cond_init(&(queue->notempty), NULL);
}

void QueueInsert(Queue *queue, int item) {
    pthread_mutex_lock(&(queue->mutex)); //locks the mutex, being sure just one thread is in operation
    while (queue->currentsize >= queue->maxsize) {
        pthread_cond_wait(&(queue->notfull), &(queue->mutex)); //we wait until queue is not full and mutex is locked at the same time
    }
    //defined queue opeartions
    queue->rear = (queue->rear + 1) % queue->maxsize;
    queue->array[queue->rear] = item;
    queue->currentsize++;
    
    //we change the conditions after insertion operation
    pthread_cond_signal(&(queue->notempty));//queue is not empty anymore
    pthread_mutex_unlock(&(queue->mutex));//mutex is unlocked after operation is done
}

int QueueRemove(Queue *queue) {
    pthread_mutex_lock(&(queue->mutex)); //locks the mutex, being sure just one thread is in operation
    while (queue->currentsize <= 0) {
        pthread_cond_wait(&(queue->notempty), &(queue->mutex));//we wait until queue is not empty and mutex is locked at the same time for operation
    }
    //defined queue operations
    int removed = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->maxsize;
    queue->currentsize--;
    //we change the conditions after operation
    pthread_cond_signal(&(queue->notfull)); //queue can not be full after removing operation
    pthread_mutex_unlock(&(queue->mutex)); //mutex is unlocked after operation is done
    return removed;//returning removed item
}

void QueueDestroy(Queue *queue) {
    free(queue->array); //deallocating memory which we created with malloc
    //we destroy thread variables
    pthread_mutex_destroy(&(queue->mutex));
    pthread_cond_destroy(&(queue->notfull));
    pthread_cond_destroy(&(queue->notempty));
}
