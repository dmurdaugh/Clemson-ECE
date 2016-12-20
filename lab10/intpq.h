class IntPQ {
  private:
  long *heap;		// An array that represent a binary heap.
  void **val;		// This is an array of associated data values that the user can provide. This is an array of void*
			// since we do not know what associated data the user may provide. This array needs to be updated
			// while swapping. Always maintain that if heap[i] = key, then its associated data is in val[i].
  void (*update)(void* data, long index);
			// This is a function pointer. The user needs to know the index to perform the decrease_key operation.
			// Once we insert elements into a priority queue, there is no way for the user to keep track of indices
			// to perform decrease_key. Hence, this update function is called whenever we change the state of the
			// heap (specifically during the swap operation) so that the user maintains a handle for the corresponding
			// element in the heap.

  long capacity;	// The total memory allocated for the heap.
  long size;		// Total number of elements currently in the heap.

				

  // The following functions returns -1 if item (left/right child/ parent) does not exist.

  // Return the index of the left-child of element stored in position index.
  long left_child (long index); // { return (2 * index + 1) < size ? (2 * index + 1) : -1; }
  // Return the index of the right-child of element stored in position index.
  long right_child (long index); // { return (2 * index + 2) < size ? (2 * index + 2) : -1; }
  // Return the index of the parent of element stored in position index.
  long parent (long index); // { return (i - 1) / 2 >= 0 ? (i - 1) / 2 : -1; }

  bool check_bounds(int index);	// Makes sure the element in position index is within the bound of the heap.
  void swap (long x, long y);		// Swaps elements at positions x and y in the heap.
  long sift_up(long index);		// sift the element up in position index.
  long sift_down(long index);		// sift the element down in position index.

  public:
  IntPQ () : heap(NULL), update(NULL), capacity(0), size(0)  {}
  IntPQ (long cp) : update(NULL), capacity(cp), size(0) {
    heap = new long[capacity];
    val = new void* [capacity];
  }
  IntPQ(long cp, void (*up)(void* data, long index)) : update(up), capacity(cp), size(0) {
    heap = new long[capacity];
    val = new void* [capacity];
  }
  ~IntPQ() { 
    delete [] heap;
    delete [] val;
   }

  long insert (long key, void* d);		// Returns the index after inserting key into the heap.
  void* remove_min ();				// remove and return the value associated with the minimum key in the heap.
  void decrease_key (long index, long new_key);	// Given the index into a valid element in the heap, decrease its key by diff.

  bool is_empty();				// Returns true if the priority queue is empty. Otherwise returns false.
  void print();				// Print the contents of the heap.
};
