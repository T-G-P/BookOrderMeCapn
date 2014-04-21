#include "queue.h"

int enqueue(struct Order_ customer_order, shmap q){
    if(q->length == QUEUE_SIZE){
        printf("Queue is full.\n");
        return 0;
    }else{
       memcpy(&(q->queue[q->end]), &customer_order, sizeof(struct Order_));
       q->length++;
       if(q->end == 9){
           q->end = 0;
       }else{
           q->end++;
       }
       return 1;
    }
}

int dequeue(Order destination, shmap q){
    if(q->length == 0){
        printf("Queue is empty.\n");
        return 0;
    }
    memcpy(destination, &(q->queue[q->start]), sizeof(struct Order_));
    q->length--;
    if(q->start == 9){
        q->start = 0;
    }else{
        q->start++;
    }
    return 1;
}
