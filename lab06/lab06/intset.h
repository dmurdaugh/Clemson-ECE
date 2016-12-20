#ifndef INTSET_H
#define INTSET_H

class Node {
  int key;
  Node *left;
  Node *right;
  Node *parent;

  public:
  Node () : key(0), left(NULL), right(NULL), parent(NULL) {}
  Node (int k) : key(k), left(NULL), right(NULL), parent(NULL) {}
  Node (int k, Node *l, Node *r) : key(k), left(l), right(r), parent(NULL) {}
  Node (int k, Node *l, Node *r, Node *p) : key(k), left(l), right(r), parent(p) {}

  friend class IntSet;
};

class IntSet {
  Node* root;
  Node* insert (Node* x, int key);
  Node* find (Node* x, int key);
  void inOrder(Node *x);
  void splay (Node *x);
  void rotate (Node *x);

  public:
  IntSet () : root(NULL) {}

  void insert (int key);
  Node* find (int key);
  void print ();
};

#endif
