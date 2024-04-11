# Prime-Number-Finder
In this project, I implement a safe queue structure and associated functions to be used by threads. The threads will be used to find prime numbers in a range of random positive numbers. The main goal is to see how threads work.
We appropriate IPC primitives (mutexes, semaphores, locks, condition variables, sleep-wait etc.) for critical section or synchronization.
Command-line options are also included in program.
By following steps, you can view a guideline for threads basicly.


# Queue Structure 
Queue is an abstract data type in which the elements are inserted to the rear and are removed from the front (first in, first out). In this project, it will be implemented as a dynamically allocated array size of which is selected from the command-line. The array will be used circularly. The actual C struct which holds the queue include the following fields:

 -The integer pointer holding the memory location of the array
 -The maximum size of the queue
 -The current size of the queue
 -The index of the front

Any additional variables directly related to the queue operation is a field of the struct. If the queue is full, the threads wanting to insert will wait until it is possible. Similarly, if the queue is empty, the threads wanting to remove will wait.
For the operation of the queue, four functions are implemented, namely:

 -QueueInitialize: Initialize the necessary fields of the queue.
 -QueueInsert: Insert an integer to the queue.
 -QueueRemove: Remove an integer from the queue.
 -QueueDestroy: Destroy the necessary fields of the queue.

The declarations and implementations of the struct and the related functions are in queue.handqueue.c files.

# Generator Thread
The program creates one generator thread. This thread generates random positive integers and inserts them to the queue. The quantity of the numbers and their possible range is decided by the command-line arguments.

After each generation, the number generated is inserted to the rear of queue. If the queue is full, then the number will be discarded. Then, the generator thread will sleep and generate as usual.

# Number Generation Rate
Between each number generation, the generator thread sleeps for a random amount of time with an exponential distribution (the rate is selected with -g option standing for generation time).
From a continuous uniform distribution x between 0 and 1, the exponential distribution can be generated as follows:

                                              𝑓(𝑦, 𝜆) = (− 1/𝜆)* ln(1 − 𝑥) 
                                              
where λ is the rate.

# Worker Thread
These threads removes a number from the queue and find whether the number is prime or not. If the number is a prime number, then the following message is going to be printed.
“Thread ID: 3059108672, Number: 79 is a prime number.”

If the number is not a prime number, then the first 10 divisors of the number is going to be printed. A result should be as follows:
“Thread ID: 3059108672, Number: 96 is not a prime number. Divisors: 1 2 3 4 6 8 12 24 32 48”
Then, worker threads will start over until the generator thread finishes working.

# Command-line arguments
The program use four optional arguments to change parameters:
 -t: Number of worker threads (default 3)
 -q: The maximum size of the queue, which is shared by the threads (default 5)
 -r: The amount of the random numbers (default 10)
 -m: The lower bound of the range of the random numbers (default 1)
 -n: The upper bound of the range of the random numbers (default 100, maximum 2000)
 -g: The rate of generation time (default 100)

# How to run in terminal?
- If you have main.c, queue.c and queue.h as source files, you can compile your code with this command:
gcc -o prime main.c queue.c –pthread
- If you have only main.c as a source file, you can compile your code with this command: gcc -o prime main.c –pthread
- Some libraries need to be linked explicitly. One example is math.h library where gcc needs -lm option.
