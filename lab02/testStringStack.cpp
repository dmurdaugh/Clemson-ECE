#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include "stringStack.h"

#define MAXCAPACITY 10

int main() {
  StringStack stk(MAXCAPACITY);
  string input;

  while (cin >> input) {
    cout << "pushing " << input << endl;
    stk.push(string(input));
  }
  cout << endl;
  cout << "top of stack " << stk.topOfStack() << endl;
  cout << endl;

  // Uncomment this line to test pushing into a full stack.
  // stk.push("Germany");

  while (!stk.isEmpty()) {
    cout << "popping " << stk.pop() << endl;
  }
  cout << endl;
  cout << "top of stack " << stk.topOfStack() << endl;

  // Uncomment this line to test popping from an empty stack.
  // stk.pop();

  return 0;
}
