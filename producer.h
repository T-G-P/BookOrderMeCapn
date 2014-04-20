#ifndef PARSE_H
#define PARSE_H
/*
 * producer.h
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "uthash.h"

struct Order_{
    char* book_title;
    float price;
    char* id;
    char* category;
};
typedef struct Order_* Order;

int producer(char*);

#endif
