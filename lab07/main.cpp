#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <algorithm>
#include <locale>

#include "stringset.h"

#define TABLESIZE 1000

// Remove some common English words.
#define COMMON(word) ((word == "the") || (word == "is") || (word == "a") || (word == "of") || (word == "to") || (word == "and") || (word == "that") || (word == "they") || (word == "he") || (word == "she") || (word == "in") || (word == "on") || (word == "at") || (word == "for") || (word == "of") || (word == "by") || (word == "are"))

// This function removes punctuation marks at the end and changes the word into strictly lower case letters.
void parse(string& word) {
  std::locale loc;

  for (std::string::size_type i = 0; i < word.length(); ++i)
    word[i] = std::tolower(word[i],loc);
  char last = word[word.length() - 1];

  if (last == '.' || last == ',' || last == '!') {
    word.resize(word.length() - 1);
  }
}

int main() {
  StringSet s(TABLESIZE);
  string word;
  Node *x;

  while (cin >> word) {
    parse(word);
    if (COMMON(word)) continue;

    // your code here
    else{
		x=s.find(word);
		if(x==NULL)
			s.insert(word,1);
		else{
			s.insert(word, x->getVal()+1);
		}
  }
}
//s.print();
  if (x = s.mostFrequent()) cout << x->getKey() << " " << x->getVal() << endl;
  return 0;
}
