#include <iostream>
#include <assert.h>
#include "intset.h"

using namespace std;

IntSet::IntSet()
{
  /* Start with dummy node */
  head = new Node(0, NULL);
}

IntSet::~IntSet()
{
  Node *temp;
  while (head != NULL) {
    temp = head;
    head = head->next;
    delete temp;
  }
}

/* Return true if key is in the set */
bool IntSet::find(int key)
{	
	Node *current = head->next;
	while(current!= NULL ){
		if(current->key == key) {
			return true;
		}
		current= current->next;
	}
return false;	 			
}

/* Inserts a new key.  It is an error if key is already in the set. */
void IntSet::insert(int key)
{
  	assert (!find(key));
	Node *newNode;	
	Node *current = head->next;
	Node *temp =head ;
	newNode= new Node(key,NULL);


	while (current != NULL) {
		if(current->key > key){
			break;
		}
		temp = current;
		current = current->next;
	}
	temp->next = newNode;
	newNode -> next= current;
}

/* Removes a key.  It is an error if key isn't in the set */
void IntSet::remove(int key)
{
 assert(find(key));

	Node *current= head ->next;
	Node *temp=head;
	
	while(current != NULL){
		if(current->key ==key){
			temp -> next =current ->next;
		}
		temp = current;
		current = current ->next;
	}	
return;
}

void IntSet::print(void)
{
	Node *current= head->next;
	Node *temp;
	while(current !=NULL){
		cout << current->key <<endl;
		current = current->next;
	}
}
