

#include "arena.h"

int main(){
  unsigned char *p, *q, *r, *s;
  arena_init();

/*  printf( "\n" );*/
/*  print_arena( 0 );*/
/*  p = alloc_from_arena( 0, 512 );*/
/*  print_arena( 0 );*/
/*  q = copy_ptr( p );*/
/*  print_arena( 0 );*/
/*  delete_ptr( p );*/
/*  print_arena( 0 );*/
/*  delete_ptr( q );*/
/*  print_arena( 0 );*/
/*  delete_ptr( p );*/
/*  print_arena( 0 );*/
/*  p = alloc_from_arena( 0, 128 );*/
/*  print_arena( 0 );*/
/*  release_arena( 0 );*/
/*  print_arena( 0 );*/

/*printf("done\n");*/
/*  printf( "\n" );*/

/*  delete_ptr( (unsigned char *)100 );*/

/*  printf( "\n" );*/

/*  print_arena( 1 );*/
/*  p = alloc_from_arena( 1, 500 );*/
/*  print_arena( 1 );*/
/*  p = alloc_from_arena( 1, 500 );*/
/*  print_arena( 1 );*/
/*  p = alloc_from_arena( 1, 500 );*/
/*  print_arena( 1 );*/
/*  p = alloc_from_arena( 1, 10 );*/
/*  print_arena_ids();*/
/*  print_arena( 1 );*/
/*  release_arena( 1 );*/
/*  print_arena( 1 );*/
/*  print_arena_ids();*/

/*  printf( "\n" );*/

/*  print_arena( 2 );*/
/*  p = alloc_from_arena( 2, 200 );*/
/*  print_arena( 2 );*/
/*  q = alloc_from_arena( 2, 200 );*/
/*  print_arena( 2 );*/
/*  r = alloc_from_arena( 2, 200 );*/
/*  print_arena( 2 );*/
/*  delete_ptr( p );*/
/*  print_arena( 2 );*/
/*  delete_ptr( r );*/
/*  print_arena( 2 );*/
/*  delete_ptr( q );*/
/*  print_arena( 2 );*/

/*  printf( "\n" );*/

  p = alloc_from_arena( 3, 1024 );
  p = alloc_from_arena( 3, 512 );
  p = alloc_from_arena( 3, 512 );
  print_arena( 3 );
  p = alloc_from_arena( 3, 1024 );
  p = alloc_from_arena( 3, 1024 );
  p = alloc_from_arena( 3, 1024 );
  print_arena( 3 );
  print_arena_ids();
  p = alloc_from_arena( 3, 1024 );
  printf( "ptr should be NULL: %p\n", p );
  release_arena( 3 );
  print_arena( 3 );
  print_arena_ids();

  printf( "\n" );

  p = alloc_from_arena( 2, 1024 );
  q = alloc_from_arena( 3, 1024 );
  r = alloc_from_arena( 2, 1024 );
  s = alloc_from_arena( 3, 1024 );
  print_arena( 2 );
  print_arena( 3 );
  print_arena_ids();
  delete_ptr( p );
  delete_ptr( q );
  delete_ptr( r );
  delete_ptr( s );
  print_arena( 2 );
  print_arena( 3 );
  print_arena_ids();

  printf( "\n" );

  printf( "end of test run\n" );

  printf( "\n" );

  return 0;
}
