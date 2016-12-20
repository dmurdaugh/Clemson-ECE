/* CPSC/ECE 3220 Summer 2016
 *
 * semaphore-based value-trading program as distributed
 *   -- note that this code has errors and must be corrected
 *
 * compile with "gcc -Wall trade_sem.c -pthread"
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>


// global variables shared between the threads

int min, max;


// initial set of semaphores to provide synchronization

sem_t min_synch, max_synch;


// helper functions

int index_of_max( int *x, int size ){
  int i;
  int max_index = 0;
  for( i = 1; i < size; i++ ){
    if( x[i] > x[max_index] ) max_index = i;
  }
  return max_index;
}

int index_of_min( int *x, int size ){
  int i;
  int min_index = 0;
  for( i = 1; i < size; i++ ){
    if( x[i] < x[min_index] ) min_index = i;
  }
  return min_index;
}

void print_values( int* a, int *b ){
  int i;
  printf( "a: %d", a[1] );
  for( i = 2; i <= a[0]; i++ ) printf( ", %d", a[i] );
  printf( "\n" );
  printf( "b: %d", b[1] );
  for( i = 2; i <= b[0]; i++ ) printf( ", %d", b[i] );
  printf( "\n" );
}


// thread 'a' code that seeks to trade for smaller values

void *minimize_a( void *arg ){
  int i;
  int size_a = *((int *) arg);  // first location is count of values
  int *a = ((int *) arg) + 1;   // a[] will be array of values
  while(1){
    i = index_of_max( a, size_a );
    max = a[i];                      // offer max for trade
    sem_post( &min_synch );
    while( sem_wait( &max_synch ) != 0 );
    if( max <= min ){// test to end trading rounds
     return NULL; 
     }   
    a[i] = min;                      // trade
    sem_post( &min_synch );
    while( sem_wait( &max_synch ) != 0 );
  }
}

// thread 'b' code that seeks to trade for larger values

void *maximize_b( void *arg ){
  int i;
  int size_b = *((int *) arg);  // first location is count of values
  int *b = ((int *) arg) + 1;   // b[] will be array of values
  while(1){
    i = index_of_min( b, size_b );
    min = b[i];                      // offer min for trade
    sem_post( &max_synch );
    while( sem_wait( &min_synch ) != 0 );
    if( max <= min ){// test to end trading rounds
     return NULL; 
     }
    b[i] = max;                      // trade
    sem_post( &max_synch );
    while( sem_wait( &min_synch ) != 0 );
  }
}

// main program

int main( int argc, char **argv ){
  pthread_t threads[2];
  int i, rc;

  int values[] = {       // format of each set of values:
                         //   count of value n, then n integer values
      7, 1,3,5,6,7,8,9,  // this set of values starts at index 0
      2, 2,4,            // starts at index 8
      5, 1,3,5,7,9,      // starts at index 11
      5, 0,2,4,6,8       // starts at index 17
  };


  // initialize semaphores

  sem_init( &min_synch, 0, 0);
  sem_init( &max_synch, 0, 0);


  // run first set of trading rounds

  printf( "-- first two sets of values --\n" );
  print_values( &values[0], &values[8] );

  rc = pthread_create( &threads[0], NULL, &minimize_a, (void *) &values[0] );
  if( rc ){ printf( "** could not create m_a thread\n" ); exit( -1 ); }

  rc = pthread_create( &threads[1], NULL, &maximize_b, (void *) &values[8] );
  if( rc ){ printf( "** could not create m_b thread\n" ); exit( -1 ); }

  for( i = 0; i < 2; i++ ){
    rc = pthread_join( threads[i], NULL );
    if( rc ){ printf( "** could not join thread %d\n", i ); exit( -1 ); }
  }

  print_values( &values[0], &values[8] );


  // run second set of trading rounds

  printf( "-- second two sets of values --\n" );
  print_values( &values[11], &values[17] );

  rc = pthread_create( &threads[0], NULL, &minimize_a, (void *) &values[11] );
  if( rc ){ printf( "** could not create m_a thread\n" ); exit( -1 ); }

  rc = pthread_create( &threads[1], NULL, &maximize_b, (void *) &values[17] );
  if( rc ){ printf( "** could not create m_b thread\n" ); exit( -1 ); }

  for( i = 0; i < 2; i++ ){
    rc = pthread_join( threads[i], NULL );
    if( rc ){ printf( "** could not join thread %d\n", i ); exit( -1 ); }
  }

  print_values( &values[11], &values[17] );


  // clean up and end program

  sem_destroy( &min_synch );
  sem_destroy( &max_synch );

  return 0;
}
