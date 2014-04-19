#include	"pthread.h"
#include	<errno.h>
#include	<stdio.h>
#include	<unistd.h>

/* Test of concurrent access controlled through mutex locks.		*/
/* Test of pthread_mutex_init(), pthread_mutex_lock(),			*/
/* pthread_mutex_unlock()						*/
// gcc -g -o thread3 thread3.c -lpthread

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int value = 0;

void *
worker( void * ignore )
{
	int	i;
	int	x;

	for ( i = 0 ; i < 1000 ; i++ )
	{
		pthread_mutex_lock( &mutex );
		x = value;
	 	sched_yield();
		value = x + 1;
		pthread_mutex_unlock( &mutex );
	}
	printf( "Thread %d ends, value is %d.\n", pthread_self(), value );
	return 0;
}

int
main()
{
	pthread_t		tid;
	int			i;

	printf( "Cogito ergo sum.\n" );
	pthread_mutex_init( &mutex, NULL );
	for ( i = 0 ; i < 2 ; i++ )
	{
		pthread_create( &tid, 0, worker, 0 );
	}
	pthread_exit( 0 );
}
