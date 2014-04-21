#ifndef CONSUMER_H
#define CONSUMER_H

/*
 * consumer.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <signal.h>

void signal_handler(int);

#endif
