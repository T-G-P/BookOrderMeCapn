#ifndef PARSE_H
#define PARSE_H
#define start_key 1300
/*
 * parse.h
 */
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include "uthash.h"
#include "queue.h"
#include "manager.h"

typedef struct Node_* Node;
extern shmap *queue;
extern char **cat_names;
extern int *pids;
extern int *shm_keys;
extern int *queueid;
extern Node customerdb_shm;
extern int customer_db_key;
extern int num_customers;
extern int num_cats;

struct Node_{
    char name[128];
    char address[256];
    char state[128];
    char zip[128];
    char id[128];
    sem_t success;
    sem_t rejection;
    float credit;
};

struct hash{
    char *key;                 /* key */
    Node customer;
    UT_hash_handle hh;         /* makes this structure hashable */
};

//Node createNode(void*,void*,void*,void*,void*,int);

//void add_node(void*, void*);

//void print_files(void);

int parse_db(char*);

int parse_categories(char*);

int number_customers(char*);
#endif
