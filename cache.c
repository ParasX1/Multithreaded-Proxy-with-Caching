//initlise_que
/*
The following cache.c has caching helper functions to allow the proxy to
access the cache queue. It has not been completely finished.
The following implmentation of LRU Caching is from the refrence links (7,8,4,13) found at the bottom of proxy.c.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csapp.h"
#include "cache.h"

/*Initialises queue which is either LRU*/
// A utility function to create an empty Queue.
// The queue can have at most 'numberOfFrames' nodes
void initialise_queue(){
    // The queue is empty
    queue.capacity = MAX_CLIENTS;
    for (int i = 0; i < queue.capacity; i++){
        queue.array[i].is_empty = 1;
        queue.array[i].no_of_readers = 0;
        Sem_init(&queue.array[i].cache_write, 0, 1);
        Sem_init(&queue.array[i].read_lock_mutex, 0, 1);
        queue.array[i].prev=NULL;
        queue.array[i].next=NULL;
    }
}


/*Enter write semaphore to write cache data*/
void enter_write_mutex(QNode node){
    P(&node.cache_write);
}

/*Lock semaphore to prevent writing cache data*/
void close_write_mutex(QNode node){
    V(&node.cache_write);
}

/*Enters the semaphore to add a count to the number of threads
waitinf to access the cache*/
void enter_read_mutex(QNode node){

    P(&node.read_lock_mutex);
    // How many people are trying to access the cache
    node.no_of_readers++;
    if (node.no_of_readers == 1){
        enter_write_mutex(node);
    }
    V(&node.read_lock_mutex);
}

/*Enters the semaphore to remove a count to the number of threads
waitinf to access the cache*/
void close_read_mutex(QNode node){
    
    P(&node.read_lock_mutex);
    // How many people are stopping their access the cache
    node.no_of_readers--;
    if (node.no_of_readers == 0)
    {
        close_write_mutex(node);
    }
    V(&node.read_lock_mutex);
}

// /*
// This function will check whether the cache is full, if the 
// cache is full, return -1 if not return 1.
// */
// int isQueueFull(){
//     for (int i = 0; i < queue.capacity; i++){
//         enter_read_mutex(queue.array[i]);
//         if (queue.array[i].is_empty == 1){
//             close_read_mutex(queue.array[i]);
//             return i;
//         }
//     }
//     return -1;
// }

// void deQueue(){
//     for (int i = 0; i < queue.capacity; i++){
//         queue.array[i++].is_empty = queue.array[i].is_empty;
//         queue.array[i++].no_of_readers = queue.array[i].is_empty;
//         strcpy(queue.array[i++].uri, "");
//         strcpy(queue.array[i++].web_buffer, "");
//     }
// }

// void cache_uri_data(char* uri, char* cache_buf){
//     if (sizeof(cache_buf) > MAX_OBJECT_SIZE){
//         return;
//     }
//     int check_if_full = isQueueFull();
//     if (check_if_full != -1){
//         queue.array[check_if_full].is_empty = 0;
//         queue.array[check_if_full].no_of_readers = 0;
//         queue.array[check_if_full].prev = (QNode*)queue.array[check_if_full--];
//         queue.array[check_if_full].next = (QNode*)queue.array[check_if_full++];
//     }
//     else {
//         deQueue();
//     }
    

// }


//todo
/*Finds an empty block in the list of objects in the cache


/*
This function will check whether the cache is full, if the 
cache is full, return -1 if not return 1.
*/
// int cache_full()


