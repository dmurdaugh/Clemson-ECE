#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using std::cout;
using std::endl;
using std::cin;

#include "stringStack.h"

// Gives the next token in the expr. 
string getToken(const string expr, int& index) {
  if (index >= expr.length()) return string("");

  // convert a string to a char *
  char * cstr;
  cstr = new char [expr.size()+1];
  strcpy (cstr, expr.c_str());

  // Tokenize, i.e., break up one line into its component tokens
  char * token;             // will hold one token from expr

  token = strtok(cstr + index, " \n");

  // Adding up index - can be used in next call
  index += string(token).length() + 1;

  if (token == NULL) return string("");
  return token;
}

// Returns true if token is an operand
bool isOperand(string token) {
  if (token.length() == 0)
    return false;
  for (int i = 0; i < token.length(); ++i) {
    if (token[i] < '0' || token[i] > '9')
      return false;
  }
  return true;
}

// Returns true if token is any of the operators ( ) * / % + -
bool isOperator(string token) {
  if (token == "(" || token == ")" || token == "*" || token == "/" || token == "%" || token == "+" || token == "-")
    return true;
  return false;
}

int getPool(string x) {
  if (x == "(") return 0;
  if (x == "+" || x == "-") return 1;
  if (x == "*" || x == "/" || x == "%") return 2;
  if (x == ")") return 3;
}
// returns the difference between the precedance of x and y. That is returns an integer > 0, if x is higher than y,
// < 0, if x is lower than y and 0 if x is equal in precedance to y. The function assumes x and y are valid operators.
int precedance(string x, string y) {
  int xpool = getPool(x);
  int ypool = getPool(y);
  return xpool - ypool;
}

// Convert the input string infix to postfix and return the postfix expression
string infixToPostfix(string infix) {
  int ind = 0;
  string token;
  string postfix("");

  // This is your stringStack object
  StringStack operStack(infix.length());

  token = getToken(infix, ind);
  while (token != string("")) {
    cout << "processing token " << token << endl;	// The next token is available. Now just process this token.

    // --- Your code starts here ---


    // --- Your code ends here ---

    token = getToken(infix, ind);
  }

  // --- Some more code here ---

  return postfix;
}

int main() {
  string expr;
  string postfix;

  getline(cin, expr);
  cout << "Infix expression: " << expr << endl;

  postfix = infixToPostfix(expr);
  cout << "Postfix expression: " << postfix << endl;

  return 0;
}
