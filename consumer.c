#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <signal.h>
#include "consumer.h"
#include "queue.h"
#include "parse.h"

static int category_key;
static int category_id;
static shmap cat;

static int customers_key;
static int customers_id;
static Node customersmap;
int num_customers;

static int exit_signal;

void signal_handler(int signum){
    if(signum == SIGINT){
        exit_signal = 1;
    }
}

int main(int argc, char **argv){
    int printed = 0;
    signal(SIGINT,signal_handler);

    category_key = atoi(argv[1]);
    customers_key = atoi(argv[2]);
    num_customers = atoi(argv[3]);
    category_id = shmget(category_key, sizeof(shmap), 0666);
    customers_id = shmget(customers_key, sizeof(customersmap), 0666);

    int key = atoi(argv[4]);
    int id = shmget(key, sizeof(int), 0666);
    int *total_money = shmat(id, NULL, 0);

    cat = shmat(category_id,NULL,0);
    customersmap = (Node)shmat(customers_id,NULL,0);

    for(;;){
        struct Order_ my_order;
        if(cat->length <= 0){
            if(exit_signal){
                break;
            }
        }

        //waiting on process to do something
        //if does not meet specified conditions continue
        if(cat->length <= 0){
            if(!printed){
                printf("Consumer is waiting...\n");
                printed = 1;
            }
            continue;
        }
        else{
            printf("Consumer is resuming...\n");
            printed = 0;
        }
        dequeue(&my_order,cat);
        //process here
        int i;
        for(i = 0; i < num_customers; i++){
            if(strcmp(my_order.id,customerdb_shm[i].id)==0){
                if(customerdb_shm[i].credit - my_order.price < 0){
                    sem_wait(&customerdb_shm[i].rejection);
                    char* cust_name = malloc(strlen(customerdb_shm[i].name) + 5);
                    sprintf(cust_name, "%s%s%s", customerdb_shm[i].name, "r", ".txt");
                    FILE* fp = fopen(cust_name, "a");
                    fprintf(fp,"%s|%.2f\n", my_order.book_title,my_order.price);
                    fclose(fp);
                    sem_post(&customerdb_shm[i].rejection);
                }
                else{
                    *total_money += my_order.price;
                    customerdb_shm[i].credit -= my_order.price;


                    sem_wait(&customerdb_shm[i].success);
                    char* cust_name = malloc(strlen(customerdb_shm[i].name) + 5);
                    sprintf(cust_name, "%s%s%s", customerdb_shm[i].name, "s", ".txt");
                    FILE* fp = fopen(cust_name, "a");

                    fprintf(fp,"%s|%.2f|%.2f\n", my_order.book_title,my_order.price,customerdb_shm[i].credit);
                    fclose(fp);
                    sem_post(&customerdb_shm[i].success);
                }
            }
        }
    }
    return 0;
}



