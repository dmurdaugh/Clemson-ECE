#include <iostream>
#include <stdlib.h>

using namespace std;

#include "intset.h"

#define SIZE	1010

int main() {
  IntSet s;
  int key;
  int arr[SIZE];
  int len = 0;

  while (cin >> key) {
    if (!s.find(key)) {
      s.insert(key);
      arr[len++] = key;
    }
  }
  cout << "Full list: ";
  s.print();		// Should print in sorted order

  // s.insert(key);	// Inserting a duplicate element. This is an error.

  int i;
  for (i = 0; i < len / 2; ++i)
    s.remove(arr[i]);
  cout << "Half list: ";
  s.print();		// Should print only the last half of the elements.

  // s.remove(arr[i-1]);	// Trying to remove an element that does not exist. This is an error.

  for (i; i < len; ++i)
    s.remove(arr[i]);
  cout << "Empty list: ";
  s.print();		// Should print an empty set.

  // s.remove(1);	// Trying to remove from an empty set. This is an error.

  return 0;
}
