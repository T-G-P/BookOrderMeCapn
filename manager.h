#ifndef MANAGER_H
#define MANAGER_H
/*
 * manager.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include "uthash.h"
#include "parse.h"
#include "queue.h"
#include "producer.h"

int valid_file(char*);

int number_categories(char*);

#endif


