#include <iostream>
using std::cout;
using std::endl;

#include <cassert>

#include "intset.h"



// Inserts key into a tree represented by x.
Node* IntSet::insert (Node* x, int key) {

if(x==NULL)
	return new Node(key);
else{
	if (key< x->key){
		x->left=insert(x->left,key);
	}
	else{
		x->right=insert (x->right,key);
	}
}
splay(x);
return x;
}

// This is a wrapper function that calls the actual insert routine. It is an error to call this function
// with a duplicate key.
void IntSet::insert (int key) {
  assert(!find(key));
  root = insert(root, key);
}

// Searches for key in tree represented by x.
Node* IntSet::find (Node *x, int key) {

if ( x == NULL ) {
	return NULL;
}
else if ( key == x->key) {
	splay(x);
	return x;
}
else if ( key < x->key) {
	return find( x->left, key );
}
else {                
	return find( x->right, key);
}
}


// This is a wrapper function that calls the actual find routine.
Node* IntSet::find (int key) {
  return find (root, key);
}

// Performs an in-order traversal.
void IntSet::inOrder (Node *x) {
if(x== NULL) 
	return;
else{
inOrder(x->left);
cout << x->key <<" ";
inOrder(x->right);
}
}

// This is a wrapper function that calls the inorder routine to enumerate contents of the set.
void IntSet::print() {
  inOrder(root);
  cout << endl;
}

void IntSet::rotate (Node *x) {
	while(x!=root){
		if(x < x->parent){
			Node* y= x->left;
			x->left= y->right;
			if(y->left)
				y->left->parent = x;
			y->parent=x->parent;
			if(!x->parent) 
				root = y;
			else if (x == x->parent->left)
				x->parent->left=y;
			else 
				x->parent->right=y;
			y->left =x;
			x->parent=y;
		}
		else{
			Node *y = x->left;
    			x->left = y->right;
    			if( y->right ){
				y->right->parent = x;
			}
    			y->parent = x->parent;
    			if( !x->parent ){
				root = y;
			}
    			else if( x == x->parent->left ) 
				x->parent->left = y;
    			else 
				x->parent->right = y;
    			y->right = x;
    			x->parent = y;
  		}
}
	}


void IntSet::splay (Node *x) {
while( x->parent ) {
      if( !x->parent->parent ) {
        if( x->parent->left == x ) 
		rotate( x->parent );
        else 
		rotate( x->parent );
      } else if( x->parent->left == x && x->parent->parent->left == x->parent ) {
        	rotate( x->parent->parent );
        	rotate( x->parent );
      } else if( x->parent->right == x && x->parent->parent->right == x->parent ) {
        	rotate( x->parent->parent );
        	rotate( x->parent );
      } else if( x->parent->left == x && x->parent->parent->right == x->parent ) {
        	rotate( x->parent );
        	rotate( x->parent );
      } else {
        	rotate( x->parent );
        	rotate( x->parent );
      }
    }
  }

