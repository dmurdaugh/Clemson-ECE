#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <fstream>
using std::ifstream;

#include "stringset.h"

int main() {
  StringSet s(1);
  string word;
  ifstream fp("words.txt");

  cout << "Building Table of all words" << endl;
  while (fp >> word) {
    s.insert(word, 0);
  }
  fp.close();

  cout << "Enter a word (press 0 to exit): " << endl;
  cin >> word;
  while (word != "0") {

    cout << "Possible alternatives for the word '"<< word << "':" << endl;

      // Your code here:

    cout << "Enter a word (press 0 to exit): " << endl;
    cin >> word;
  }
  return 0;
}
