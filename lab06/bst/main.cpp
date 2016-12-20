#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include "intset.h"

#include <cstdio>
#include <time.h>
#include <cstdlib>

#define SIZE 10000
#define START1 100
#define START2 1000
#define END 10000
#define STEP1 100
#define STEP2 1000

int main() {
  int key;
  IntSet s;
  timespec ts;
  unsigned long long nano;
  unsigned long long milli;

  cout << "Inserting " << SIZE << " elements" << endl;
  for (int i = 0; i < SIZE; ++i) s.insert(i);		// Insert about 100000 keys.

  for (int i = START1; i <= END; ) {
    ts.tv_sec = 0;
    ts.tv_nsec = 0;
    clock_settime(CLOCK_PROCESS_CPUTIME_ID, &ts);

    for (int j = 0; j < i; ++j) {			// Find a random key for i times.
      s.find(rand() % SIZE);
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
    nano = (unsigned long long) (ts.tv_sec * 1000000000) + (unsigned long long) ts.tv_nsec;
    milli = nano / 1000000;

    cout << i << " find operations " << milli  << " milli seconds" << endl;

    i += (i < START2) ? STEP1 : STEP2 ;
  }
  return 0;
}
