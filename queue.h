#ifndef QUEUE_H
#define QUEUE_H
/*
 * queue.h
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <semaphore.h>
#include "uthash.h"

#define QUEUE_SIZE 10

struct Order_{
    char book_title[256];
    float price;
    char id[128];
    char category[128];
};
typedef struct Order_* Order;

struct shm_map_{
    char name[128];     //Name of the category
    sem_t lock;   //used to lock the shared memory (only one thing can access it at a time
    struct Order_ queue[QUEUE_SIZE];  //The queue is an array of orders
    int start;  //where the queue starts (pointer to an order in the queue
    int end;    //index where the queue ends
    int length; //length of the queue
};
typedef struct shm_map_* shmap;

int enqueue(Order, shmap);    //adds order to the queue;

int dequeue(Order, shmap);    //removes whatever's at the start of the queue, puts it into Order, Order must be malloc'd before
#endif
