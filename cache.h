/*The following Header file is for creating a block which is in a queue
for caching.
The following implmentation of LRU Caching is from the refrence links (7,8,4,13) found at the bottom of proxy.c.
*/

#ifndef COMP2310_CACHE_H
#define COMP2310_CACHE_H

#include <stdlib.h>
#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000 // MAX_CACHE_SIZE not used as did not understand the concept of the max cache size
#define MAX_CLIENTS 10
#define MAX_OBJECT_SIZE 102400

// A Queue Node (Queue is implemented using Doubly Linked List)
typedef struct QNode {
    char web_buffer[MAX_OBJECT_SIZE];
    int no_of_readers;
    int is_empty;
    char uri[MAXLINE];
    sem_t read_lock_mutex; // allows the number thread to access the number of readers
    sem_t cache_write; // Allows write using cache_write
    struct QNode *prev, *next;

} QNode;
  
// A Queue (A FIFO collection of Queue Nodes)
typedef struct Queue {
    int capacity;
    QNode *front, *rear;
    QNode array[MAX_CLIENTS];
} Queue;

Queue queue;
  
// Attempt at creating a hash table, for cashing leaves
// // A hash (Collection of pointers to Queue Nodes)
// typedef struct Hash {
//     int capacity; // how many pages can be there
//     QNode** array; // an array of queue nodes
// } Hash;

#endif
