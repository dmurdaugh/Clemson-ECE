#include <string>
using std::string;

class Node {
  private:
  string key;
  int val;
  Node *next;

  public:
  Node () : key(""), val(0), next(NULL) {}
  Node (string k, int v) : key(k), val(v), next(NULL) {}
  Node (string k, int v, Node *n) : key(k), val(v), next(n) {}

  void setVal (int v) { val = v; }
  int getVal () { return val; }
  string getKey () { return key; }

  friend class StringSet;
};

class StringSet {
  private:
  int table_size;
  int num_elements;
  Node **hash_table;

  int hash(string s);

  public:
  StringSet () : table_size(1),  num_elements(0) {}
  StringSet (int ts) : num_elements(0) {
    table_size = (ts <= 0) ? 1 : ts;
    hash_table = new Node* [table_size];
    for (int i = 0; i < table_size; ++i) {
      hash_table[i] = new Node("0", 0);		// Dummy Nodes
    }
  }
  ~StringSet () {
    for (int i = 0; i < table_size; ++i) {
      Node *x = hash_table[i];
      while (x != NULL) {
	Node *temp = x;
	x = x->next;
	delete temp;
      }
    }
    delete [] hash_table;
  }
  
  void insert(string key, int val);
  Node* find(string key);
  void print();
  Node* mostFrequent();
};
