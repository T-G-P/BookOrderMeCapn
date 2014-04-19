#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *myfunc(void *myvar);

void *myfunc (void *myvar){
    char *msg;
    msg = (char *) myvar;

    int i;
    for(i = 0; i<10; i++){
        printf("%s %d\n", msg, i);
        sleep(1);
    }

    return NULL;
}

int main(int argc, char** argv){

    pthread_t thread1, thread2;
    char *msg1 = "first thread";
    char *msg2 = "second thread";
    int ret1, ret2;

    /*Passing in the address of the thread, passing in the attributes (default),
     * the starting function, and the parameters being sent to the function
     */
    ret1 = pthread_create(&thread1, NULL, myfunc, (void *) msg1);
    ret2 = pthread_create(&thread2, NULL, myfunc, (void *) msg2);

    printf("Main function after pthread_create\n");

    /*Waits until the specified thread to finish before moving on to the next one*/
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("first thread ret1 = %d\n", ret1);
    printf("Second thread ret2 = %d\n",ret2);

    return 0;
}

