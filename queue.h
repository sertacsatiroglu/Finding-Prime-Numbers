#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h> 
// we use pthread since queue implementation involves multi-threading
//we define required parameters for queue structure
typedef struct {

    int *array;
    int maxsize;  //max array size
    int currentsize;  // current size of array
    int front;  //front index for applications in queue
    int rear; // rear index for applications in queue
    int lowbound;     // lower bound of range of random numbers
    int upbound;      // upper bound of range of random numbers
    int numnumbers;   // amount of random numbers
    int rate;         // the rate generation of time
    
    pthread_mutex_t mutex; //by using mutex, we ensure that only one thread can access resources, namely for safety.
    pthread_cond_t notfull; //indicates if queue is full or not 
    pthread_cond_t notempty; //indicates if queue is empty or not 
} Queue;

//we define will define functions applied on queue
void QueueInsert(Queue *queue, int item);
void QueueInitialize(Queue *queue, int maxsize, int lower_bound, int upper_bound, int num_numbers, int generation_rate);
int QueueRemove(Queue *queue);
void QueueDestroy(Queue *queue);

#endif
