#include    <errno.h>
#include  <malloc.h>
#include  <pthread.h>
#include  <semaphore.h>
#include  <stdio.h>
#include  <string.h>
#include  <unistd.h>

// Producer-Consumer example program using sempaphores.

#define   FALSE     0
#define TRUE   1
#define SBUFSIZE        16

struct SharedData {
    int          isopen;
    int       refcount; // reference count:  number of threads using this object
    unsigned int    front;        // subscript of front of queue
    unsigned int    count;        // number of chars in queue
    unsigned int    bufsize;
    char            buf[SBUFSIZE];
    sem_t          empty_count;
    sem_t          full_count;
    sem_t          use_queue;     // mutual exclusion
};

void initialize( struct SharedData * sptr, int rc )         // Looks like a ctor
{
    sptr->isopen = TRUE;
    sptr->refcount = rc;
    sptr->front = 0;
    sptr->count = 0;
    sptr->bufsize = SBUFSIZE;
    sem_init( &sptr->empty_count, 0, SBUFSIZE );
    sem_init( &sptr->full_count, 0, 0 );
    sem_init( &sptr->use_queue, 0, 1 );
}

void *consumer( void * arg )
{
    struct SharedData * d = (struct SharedData *)arg;
    char           c;

    pthread_detach( pthread_self() );
    while ( d->isopen )
    {
        sem_wait( &d->full_count );
        sem_wait( &d->use_queue );
        c = d->buf[d->front];
        printf( "consumer gets >%c<\n", c );
        d->front = (d->front + 1) % d->bufsize;
        --d->count;
        sleep( 1 );         // pretend to do something
        sem_post( &d->use_queue );
        sem_post( &d->empty_count );
    }
    sem_wait( &d->use_queue );
    while ( d->count > 0 )                  // Now get remaining stuff from buffer
    {
        c = d->buf[d->front];
        printf( "consumer gets >%c<\n", c );
        d->front = (d->front + 1) % d->bufsize;
        --d->count;
    }
    if ( --d->refcount <= 0 )               // A new memory management model.
    {
        printf( "consumer free()s shared data.\n" );
        sem_destroy( &d->use_queue );
        sem_destroy( &d->empty_count );
        sem_destroy( &d->full_count );
        free( d );
    }
    else
    {
        sem_post( &d->use_queue );
    }
    return 0;
}

void *producer( void * arg )
{
    struct SharedData * d = (struct SharedData *)arg;
    int            i;
    int            limit;
    int            back;
    char           p2[100];

    pthread_detach( pthread_self() );
    while ( printf( "Enter something->" ), scanf( " %[^\n]\n", p2 ) > 0 )
    {
        limit = strlen( p2 );
        for ( i = 0 ; i < limit ; i++ )
        {
            sem_wait( &d->empty_count );
            sem_wait( &d->use_queue );
            back = (d->front + d->count) % d->bufsize;
            d->buf[back] = p2[i];
            ++d->count;
            printf( "producer puts >%c< there are %d chars in queue.\n", p2[i], d->count );
            sem_post( &d->use_queue );
            sem_post( &d->full_count );
        }
    }
    printf( "BKR EOF for producer line %d\n", __LINE__ );
    sem_wait( &d->use_queue );
    d->isopen = FALSE;
    if ( --d->refcount <= 0 )               // A new memory management model.
    {
        printf( "producer free()s shared data.\n" );
        sem_destroy( &d->use_queue );
        sem_destroy( &d->empty_count );
        sem_destroy( &d->full_count );
        free( d );
    }
    else
    {
        sem_post( &d->use_queue );
    }
    return 0;
}

int main( int argc, char ** argv )
{
    pthread_t      ignore;
    char *              func = "main";
    struct SharedData * sharedData;

    if ( (sharedData = (struct SharedData *)malloc( sizeof(struct SharedData) )) == 0 )
    {
        printf( "malloc() failed in %s.\n", func );
        return 1;
    }
    else
    {
        initialize( sharedData, 2 );
        pthread_create( &ignore, 0, consumer, sharedData );
        pthread_create( &ignore, 0, producer, sharedData );
        pthread_exit( 0 );       // Look Ma, no leaks ...
    }
}

