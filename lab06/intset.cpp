#include <iostream>
using std::cout;
using std::endl;

#include <cassert>

#include "intset.h"



// Inserts key into a tree represented by x.
Node* IntSet::insert (Node* x, int key) {
  int key_value;
  node *left;
  node *right;
  if(key< x->key_value)
  {
    if(x->left!=NULL)
     insert(key, x->left);
    else
    {
      x->left=new node;
      x->left->key_value=key;
      x->left->left=NULL;    //Sets the left child of the child node to null
      x->left->right=NULL;   //Sets the right child of the child node to null
    }  
  }
  else if(key>=x->key_value)
  {
    if(x->right!=NULL)
      insert(key, x->right);
    else
    {
      x->right=new node;
      x->right->key_value=key;
      x->right->left=NULL;  //Sets the left child of the child node to null
      x->right->right=NULL; //Sets the right child of the child node to null
    }
  }
}

// This is a wrapper function that calls the actual insert routine. It is an error to call this function
// with a duplicate key.
void IntSet::insert (int key) {
  assert(!find(key));
  root = insert(root, key);
}

// Searches for key in tree represented by x.
Node* IntSet::find (Node *x, int key) {
{
                // Return true if item is one of the items in the binary
	                 // sort tree to which root points.   Return false if not.
	             if ( root == NULL ) {
	                   // Tree is empty, so it certainly doesn't contain item.
	                return false;
	             }
	             else if ( x == root->x ) {
	                   // Yes, the item has been found in the root node.
	                return true;
	             }
	             else if ( x < root->key_value ) {
	                   // If the item occurs, it must be in the left subtree.
	                return search( x, root->left );
	             }
	             else {
	                   // If the item occurs, it must be in the right subtree.	                
			return search( x, root->right );
	             }
	          }


}
}

// This is a wrapper function that calls the actual find routine.
Node* IntSet::find (int key) {
  return find (root, key);
}

// Performs an in-order traversal.
void IntSet::inOrder (Node *x) {
if (!x) return;
  in_order_traversal(x->left);
  cout << x->data;
  in_order_traversal(x->right);
}

// This is a wrapper function that calls the inorder routine to enumerate contents of the set.
void IntSet::print() {
  inOrder(root);
  cout << endl;
}

void IntSet::rotate (Node *x) {
// Fill your code here
}



void IntSet::splay (Node *x) {
while( x->parent ) {
      if( !x->parent->parent ) {
        if( x->parent->left == x ) 
		right_rotate( x->parent );
        else 
		left_rotate( x->parent );
      } else if( x->parent->left == x && x->parent->parent->left == x->parent ) {
        	right_rotate( x->parent->parent );
        	right_rotate( x->parent );
      } else if( x->parent->right == x && x->parent->parent->right == x->parent ) {
        	left_rotate( x->parent->parent );
        	left_rotate( x->parent );
      } else if( x->parent->left == x && x->parent->parent->right == x->parent ) {
        	right_rotate( x->parent );
        	left_rotate( x->parent );
      } else {
        	left_rotate( x->parent );
        	right_rotate( x->parent );
      }
    }
  }
}
