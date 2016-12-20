
#include<stdio.h>
#include<stdlib.h>

#define N_ARENAS 4
#define N_SLABS 8
#define N_BYTES 1024

struct slab_struct{
  struct slab_struct *next;
  unsigned char *ptr;
  int arena_id;
  unsigned char bytes[N_BYTES];
};

struct slab_struct slab[N_SLABS];
struct slab_struct *free_stack;

struct slab_struct *arena[N_ARENAS];
int arena_ref_cnt[N_ARENAS];

void arena_init( void );

int release_arena( int );
unsigned char *alloc_from_arena( int, int );
unsigned char *copy_ptr( unsigned char* );
void delete_ptr( unsigned char* );

void print_arena( int );
void print_arena_ids( void );
