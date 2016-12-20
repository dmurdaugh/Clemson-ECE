

#include "arena.h"

void push_free_stack( struct slab_struct *slab ){

  // add necessary code
  struct slab_struct *rover=free_stack;
  struct slab_struct *dover=slab;
  struct slab_struct *end;
  if(slab==NULL)
   return;
  while(dover->next!=NULL){
  	dover=dover->next;
  }
  end=dover;
  while(rover!=NULL && rover->next!=NULL){
  	rover=rover->next;
  }
  dover=free_stack;
  
  if (free_stack==NULL){
   free_stack=slab;
  }
  else{
  	rover->next=slab;
  }
  
  rover=free_stack;
  while(rover != end){
  	rover=rover->next;
  }
  rover->next=NULL;
}


struct slab_struct *pop_free_stack( int arena_id ){
  struct slab_struct *return_value = free_stack;
  struct slab_struct *rover = free_stack;
  // add necessary code
  if(return_value->next==NULL){
  	free_stack=NULL;
  	return return_value;
  }
  if(return_value->next!=NULL){
  	return_value=return_value->next;
  }
  while(return_value->next!=NULL){
  	return_value=return_value->next;
  	rover=rover->next;
  }
  	return_value->arena_id=arena_id;
  	rover->next=NULL;
  	return_value->next=NULL;
  return return_value;
}


void arena_init(){
  int i;
  for( i = 0; i < N_SLABS; i++ ){
    slab[i].next = NULL;
    slab[i].ptr = &(slab[i].bytes[0]);
    slab[i].arena_id = -1;
  }
  for( i = 0; i < N_ARENAS; i++ ){
    arena[i] = &slab[i];
    slab[i].arena_id = i;
    arena_ref_cnt[i] = 0;
  }
  free_stack = NULL;
  for( i = N_ARENAS; i < N_SLABS; i++ ){
    push_free_stack( &slab[i] );
  }
}


int release_arena( int arena_id ){
  struct slab_struct *current;
  if( (arena_id < 0) || (arena_id >= N_ARENAS) ){
    printf( "release: bad arena_id\n" );
    return 1;
  }
  if( arena[arena_id] == NULL ){
    printf( "release: null arena ptr\n" );
    return 2;
  }
  if( arena_ref_cnt[arena_id] > 0 ){
    printf( "warning: releasing a region with %d references\n",
      arena_ref_cnt[arena_id] );
  }
  // add code to reset all slabs and return all but first to the free list
  // (traverse singly-linked slab list and push slabs in that order)
  current = arena[arena_id]->next;
  	
  while( current != NULL ){
    current->arena_id=-1;
    current->ptr=&current->bytes[0];
    current = current->next; 
  }
  current=arena[arena_id]->next;
  push_free_stack(current);
  // remember to set the arena_id fields of freed slabs to -1
  current=arena[arena_id];
  current->ptr=&current->bytes[0];
  arena_ref_cnt[arena_id]=0;
  current->next=NULL;
    
  return 0;
}


unsigned char *alloc_from_arena( int arena_id, int size ){
  struct slab_struct *current;
  unsigned char *return_ptr;
  if( (arena_id < 0) || (arena_id >= N_ARENAS) ){
    printf( "alloc: bad arena_id\n" );
    return NULL;
  }
  if( size > N_BYTES ){
    printf( "alloc: requested size too large\n" );
    return NULL;
  }
  current = arena[arena_id];
  if(current->next==NULL){
	  if(size <= &current->bytes[N_BYTES] - current->ptr){
			return_ptr= current->ptr+size;
			current->ptr=return_ptr;
			arena_ref_cnt[arena_id]++;
			return return_ptr;
	   }
  }
  else{
  	while(current->next!=NULL){
  	  if(size <= &current->bytes[N_BYTES] - current->ptr){
			return_ptr= current->ptr+size;
			current->ptr=return_ptr;
			arena_ref_cnt[arena_id]++;
			return return_ptr;
	   }
	   current=current->next;
  	}
  }
  if(size <= &current->bytes[N_BYTES] - current->ptr){
			return_ptr= current->ptr+size;
			current->ptr=return_ptr;
			arena_ref_cnt[arena_id]++;
			return return_ptr;
	   }
  // add code to return a pointer if space is available, otherwise
  //   return NULL
  if(free_stack!=NULL){
		current->next=pop_free_stack(arena_id);
		current=current->next;
		current->next=NULL;
		current->arena_id=arena_id;
		if(size <= &current->bytes[N_BYTES] - current->ptr){
			return_ptr= current->ptr+size;
			current->ptr=return_ptr;
			arena_ref_cnt[arena_id]++;
			return return_ptr;
		}
	}
	current = arena[arena_id];
	while(current!=NULL){
		current=current->next;
	}
	current =arena[arena_id];
	return NULL;
  // remember to set the arena_id field of an added slabs to the value
  //   of the arena specified by the input parameter

}


// return input argument so usage can be p2 = copy_ptr( p1 );
unsigned char* copy_ptr( unsigned char *ptr ){
  int i, arena_id;
  for( i = 0; i < N_SLABS; i++ ){
    if(ptr >= &slab[i].bytes[0] && ptr <= &slab[i].bytes[N_BYTES]){
		
          // add code to determine if the pointer falls into
          //   an allocated area within this slab
      arena_id = slab[i].arena_id;
      if( (arena_id < 0) || (arena_id >= N_ARENAS) ){
        printf( "copy ptr: bad arena_id\n" );
        return NULL;
      }
      arena_ref_cnt[arena_id]++;
      return ptr;
    }
  }
  printf( "copy ptr: ptr was not within an allocated area in a slab\n" );
  return NULL;
}


void delete_ptr( unsigned char *ptr ){
  int i, arena_id;
  for( i = 0; i < N_SLABS; i++ ){
    if(ptr >= &slab[i].bytes[0] && ptr <= &slab[i].bytes[N_BYTES]){
		
          // add code to determine if the pointer falls into
          //   an allocated area within this slab
      arena_id = slab[i].arena_id;
      if( arena_ref_cnt[arena_id] <= 0 ){
        printf( "delete ptr: bad ref count\n" );
      }else{
        arena_ref_cnt[arena_id]--;
      }
      if( arena_ref_cnt[arena_id] == 0 ) release_arena( arena_id );
      return;
    }
  }
  printf( "delete ptr: ptr was not within an allocated area in a slab\n" );
}


void print_arena( int arena_id ){
  struct slab_struct *current;
  if( (arena_id < 0) || (arena_id >= N_ARENAS) ){
    printf( "print arena: bad arena_id\n" );
    exit(-1);
  }
  printf( "a%d: allocated spaces", arena_id );
  current = arena[arena_id];
  while( current != NULL ){
    printf( " %d,", (int)(current->ptr - &current->bytes[0]) );
    current = current->next;
  }
  printf( " refs=%d\n", arena_ref_cnt[arena_id] );
}


void print_arena_ids(){
  int i;
  printf( "slab arena_id values:" );
  for( i = 0; i < N_SLABS; i++ ){
    printf( " %d", slab[i].arena_id );
  }
  printf( "\n" );
}
