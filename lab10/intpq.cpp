#include <iostream>
using std::cout;
using std::endl;

#include <assert.h>
#include "intpq.h"

long IntPQ::left_child (long index) { 
  return (2 * index + 1) < size ? (2 * index + 1) : -1;
}
long IntPQ::right_child (long index) { 
  return (2 * index + 2) < size ? (2 * index + 2) : -1;
}
long IntPQ::parent (long index) {
  return (index - 1) / 2 >= 0 ? (index - 1) / 2 : -1;
}

bool IntPQ::check_bounds(int index) {
  return (index >= 0 && index < size);
}

void IntPQ::swap (long x, long y) {
  assert(check_bounds(x) && check_bounds(y));

  long temp = heap[x];
  heap[x] = heap[y];
  heap[y] = temp;

  void *ptrtemp = val[x];
  val[x] = val[y];
  val[y] = ptrtemp;

  if (update != NULL) {
      update(val[x], x);
      update(val[y], y);
  }
}

long IntPQ::sift_up (long index) {
  assert(check_bounds(index));
  int temp;
  //while(heap[index] < heap[parent(index)]){
  for(temp=index;temp !=0 and heap[temp] < heap[parent(index)];temp=parent(temp));
  	swap(temp,parent(temp));
	return temp;
}

long IntPQ::sift_down (long index) {
  assert(check_bounds(index));
 	int temp;
 	int final;
  if(right_child(index)==-1 && left_child(index)==-1){
  	return index;
  }
  if(left_child(index) != -1 && right_child(index) == -1){
  	final= left_child(index);
  	swap(index, left_child(index));
  	}
  else if(heap[left_child(index)] < heap[right_child(index)]){
  	final= left_child(index);
  	swap(index, left_child(index));
  	}
  else{
  	final= right_child(index);
  	swap(index, right_child(index));
  }
return final;
}

long IntPQ::insert (long key, void* d) {
  assert(size < capacity - 1);
	void *ptrcmp=d;
	heap[size]=key;
	val[size]=d;
	size++;
return sift_up(size-1);
}

void* IntPQ::remove_min () {
  assert(size > 0);
  void* temp=val[0];
  long left,right;
  long index =0;
  swap(size-1,index);
  size--;
  left= left_child(index);
  right= right_child(index);
   
  while((left !=-1 && heap[left] <heap [index]) || (right !=-1 && heap[right] <heap [index])){
  	index= sift_down(index);
  	left= left_child(index);
  	right= right_child(index);
  }
return temp;
}

void IntPQ::decrease_key (long index, long new_key) {
  assert(check_bounds(index) && new_key < heap[index]);
	heap[index]=new_key;
		sift_up(index);
	}

bool IntPQ::is_empty() {
  return (size == 0) ? true : false;
}

void IntPQ::print() {
  for (long i = 0; i < size; ++i)
    cout << heap[i] << " ";
  cout << endl;
}

