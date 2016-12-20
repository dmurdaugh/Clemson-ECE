#ifndef INTSET_H /* Prevent multiple inclusion... */
#define INTSET_H

class Node {

  friend class IntSet;

  private:
  int key;
  Node *next;

  public:
  Node() : key(0) { next = NULL; }
  Node(int k, Node *n) : key(k), next(n) {}
};

class IntSet {

  private: 
  Node *head;

  public:
  IntSet();
  ~IntSet();
  bool find(int key);
  void insert(int key);
  void remove(int key);
  void print(void);
};

#endif
