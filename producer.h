#ifndef PRODUCER_H
#define PRODUCER_H
/*
 * producer.h
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include "uthash.h"
#include "queue.h"
#include "parse.h"

//extern int num_cats;
//extern char **cat_names;
//extern shmap *queue;

void * producer(void*);

void write_order(Order);

#endif
