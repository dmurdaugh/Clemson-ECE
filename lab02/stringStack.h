#include <string>
using std::string;

class StringStack {
  private:
  string *stack;		// An array of strings that can be used to represent a stack of strings
  int capacity;		// The total capacity allocated for the stack
  int top;			// Logically points to the top of the stack
  
  public:
  StringStack() : capacity(0), top(0) {};
  StringStack(const int& cp) : capacity(cp), top(0) {
    stack = new string [capacity];
  }
  StringStack(const StringStack& copy) : capacity(copy.capacity), top(copy.top) {
    stack = new string [capacity];
    for (int i = 0; i < capacity; ++i)
      stack[i] = copy.stack[i];
  }
  ~StringStack()  {
    delete [] stack;
  }

  void push(const string& x);
  string pop();
  string topOfStack();
  bool isEmpty();
};
