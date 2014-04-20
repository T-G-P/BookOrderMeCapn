#ifndef PARSE_H
#define PARSE_H
/*
 * parse.h
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include "uthash.h"


struct Node_{
    void* name;
    void* address;
    void* state;
    void* zip;
    void* id;
    float credit;
};
typedef struct Node_* Node;

struct hash{
    char *key;                 /* key */
    Node customer;
    UT_hash_handle hh;         /* makes this structure hashable */
};

Node createNode(void*,void*,void*,void*,void*,int);

void add_node(void*, void*);

void print_files(void);

int parse_db(char*);

int parse_categories(char*);

#endif

