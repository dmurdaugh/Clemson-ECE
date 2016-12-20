// bst.c
// Denzel Murdaugh
// MP5
// ECE 223

 
#define TRUE 1
#define FALSE 0
 
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include "bst.h"
 
int freq_calls; //global variable to hold number of key comparisons
 
//Function prototypes
bst_node_t *insert_find(bst_node_t* node, bst_key_t key);
void node_destruct(bst_node_t *node);
data_t bst_data(bst_node_t* node);
int bst_check(bst_node_t* node, int min, int max);
bst_node_t *find_node(bst_node_t* node, bst_key_t key);
bst_node_t *find_parent(bst_node_t* node, bst_key_t key, int *dir);
int num_child(bst_node_t *node);
bst_node_t *aux_remove(bst_node_t **r_node);
bst_node_t *remove_none(bst_node_t** r_node);
bst_node_t *remove_left(bst_node_t** r_node);
bst_node_t *remove_right(bst_node_t** r_node);
bst_node_t *remove_two_child(bst_node_t** r_node);
void level_find(bst_node_t *node, int level, int *max_lvl);
void node_analysis(bst_node_t *node, int **array, int v_pos, int h_pos, int *max_digits);
int InternalPathLength(bst_node_t* node,int value);
void print_bst(int **array, int max_lvl, int max_digits);
int digit_val(int n);
void space_format(int n);
void print_value(int r, int digits);
int getBalance(bst_node_t *N);
bst_node_t *leftRotate(bst_node_t *r);
bst_node_t *rightRotate(bst_node_t *l);
int height_check(bst_node_t *N);
bst_node_t *AVL_insert(bst_t*,bst_node_t*,int key);
int comp(int a, int b);


//find the tree element with the matching key and return the pointer
data_t bst_access(bst_t* tree, bst_key_t key){
    freq_calls = 0;
    data_t ret_data;
    ret_data = bst_data(find_node(tree->root,key));
    if (ret_data==NULL){
    	//printf("no seg fault\n");
    }
    tree->num_key_recent_comparisons = freq_calls;
    return ret_data;
}
 
// creates the tree starting at the header block
// also saves the tree policy in the header block
bst_t *bst_construct(int tree_policy){
    bst_t *tree = (bst_t*) malloc(sizeof(bst_t));
    tree->root = NULL;
    tree->tree_policy = tree_policy;
    tree->tree_size = 0;
    tree->num_key_recent_comparisons = 0;
    return tree;
}
 
// deconstructs the tree freeing all the nodes
void bst_destruct(bst_t* tree){
    node_destruct(tree->root);
    free(tree);
}
 
// Private function: goes node by node recursively freeing the nodes
void node_destruct(bst_node_t *node){
    if(node->left != NULL)
        node_destruct(node->left);
    if(node->right != NULL)
        node_destruct(node->right);
    free(node);
}
 
// Insert the node into the tree
int bst_insert(bst_t* tree, bst_key_t key, data_t data_ptr){
    freq_calls = 0;
    bst_node_t *node = insert_find(tree->root,key);
    tree->num_key_recent_comparisons = freq_calls;
    bst_node_t *new_node=NULL;
	if (tree->tree_policy==AVL){
		if (node == NULL)
		    tree->root = new_node;
		else if (node->key == key){
		    free(new_node);
		    return 0;
		}   

			tree->root=AVL_insert(tree,tree->root,key);
			
		}
    else{
    new_node = (bst_node_t*) malloc(sizeof(bst_node_t));
    new_node->data_ptr = data_ptr;
    new_node->left = new_node->right = NULL;
    new_node->key = key;
	new_node->height=1;
    	
    if (node == NULL)
        tree->root = new_node;
    else if (node->key == key){
        free(new_node);
        return 0;
    }   
	else if(node->key > key)
        node->left = new_node;
    else    
        node->right = new_node;
    }
    tree->tree_size += 1;
    
    return 1;
}
 
// Private function Finds the node related to the key so that a child could be made
bst_node_t *insert_find(bst_node_t* node, bst_key_t key){
    if(node == NULL) 
        return NULL;
    if (key > node->key){
        freq_calls++;
        if(node->right != NULL)
            return insert_find(node->right,key);
        else
            return node;
    }
    else if(key < node->key){
        freq_calls++;
        if(node->left != NULL)
            return insert_find(node->left,key);
        else
            return node;
    }
    else{ 
        freq_calls++;
        return node;
    }
}
 
// Remove the item in the tree with the matching key. Return the pointer to
// removed node
data_t bst_remove(bst_t* tree, bst_key_t key){
    freq_calls = 0;
    int direction = 0; 
    bst_node_t *parent = find_parent(tree->root,key,&direction);
    data_t ret_data;
 
    if(direction == 0){ 
        if(tree->root->key != key){ 
            tree->num_key_recent_comparisons = freq_calls;
            return NULL;
        }
        ret_data = tree->root->data_ptr;
        tree->root = aux_remove(&(tree->root));
    }
    else if(direction == -1){
        bst_node_t *r_node = parent->left;
        ret_data = r_node->data_ptr;
        parent->left = aux_remove(&r_node);
    }
    else{
        bst_node_t *r_node = parent->right;
        ret_data = r_node->data_ptr;
        parent->right = aux_remove(&r_node);
    }
    tree->tree_size -= 1;
    tree->num_key_recent_comparisons = freq_calls;
    return ret_data;
}
 
// Private function: Chooses a proper remove function based on the number of children
// the r_node has. Assumes given node is not NULL. Returns node that takes
// the old position of the deleted r_node. 
bst_node_t *aux_remove(bst_node_t **r_node){
    assert(r_node != NULL);
    switch(num_child(*r_node)){
        case 0:
            return remove_none(r_node);
        case 1:
            return remove_left(r_node);
        case 2:
            return remove_right(r_node);
        case 3:
            return remove_two_child(r_node);
        default:
            printf("ERROR: num_child() WITHIN aux_remove()\n");
            exit(1);
    }
}
 
// private function: frees the r_node and replaces it.
bst_node_t *remove_none(bst_node_t** r_node){
    free(*r_node);
    return NULL;
}
 
// private function: frees the r_node and replaces it.
bst_node_t *remove_left(bst_node_t** r_node){
    //find greatest key less than r_node
    int dummy;
    bst_node_t *new_node = insert_find(*r_node,(*r_node)->key - 1);
    bst_node_t *parent = find_parent(*r_node,new_node->key,&dummy);
    if(*r_node != parent && new_node->left != NULL){
        parent->right = new_node->left;
    }
    free(*r_node);
    *r_node = new_node;
    return new_node;
}
 
// private function: frees the r_node and replaces it.
bst_node_t *remove_right(bst_node_t** r_node){
    bst_node_t *new_node = (*r_node)->right;
    free(*r_node);
    *r_node = new_node;
    return new_node;
}
 
// private function: frees the r_node and replaces it.
bst_node_t *remove_two_child(bst_node_t** r_node){
    bst_node_t *right_node = (*r_node)->right;
    bst_node_t *new_node = remove_left(r_node);
    new_node->right = right_node;
    return new_node;
}
 
//returns size of the tree
int bst_size(bst_t* tree){
    return tree->tree_size;
}
 
// Return num_key_recent_comparisons
int bst_stats(bst_t* tree){
    return tree->num_key_recent_comparisons;
}
 
//return path of integral list
int bst_int_path_len(bst_t *tree){
    return InternalPathLength(tree->root,0);
}

int InternalPathLength(bst_node_t* node,int value){
	if(node == NULL) 
{
    return 0;
}
return (value+InternalPathLength(node->right,value+1)+InternalPathLength(node->left,value+1));
}
 
/* Prints out a graphical represntation of the given BST. Function does nothing
 * if tree is empty.
 */
void bst_debug_print_tree(bst_t* tree){
    int i, j, max_lvl = 0, max_digits = 0;
    level_find(tree->root,-1,&max_lvl);
    if(max_lvl == 0){//tree is empty
        return;
 	}
    int **array;
    array = (int**) malloc(sizeof(int*)*max_lvl); 
    for (i = 0; i <= max_lvl; i++)
    {
        array[i] = (int*) malloc(sizeof(int)*pow(2,i));
        for (j = 0; j < pow(2,i); j++){
            array[i][j] = -1;
        }
    }
    node_analysis(tree->root,array,0,0,&max_digits);
    print_bst(array,max_lvl,max_digits);
 

    for (i = 0; i <= max_lvl; i++)
    {
     	printf("free %d\n",i);
     	if(tree->tree_policy==BST)
        free(array[i]);  
    }
    free(array);
}
 
// Private function: Determines the depth below the given node. stores in max_lvl

void level_find(bst_node_t *node, int level, int *max_lvl){
    if(node == NULL){
        if(level > *max_lvl)
            *max_lvl = level;
        return;
    }
    level_find(node->left,level+1,max_lvl);
    level_find(node->right,level+1,max_lvl);     
}
 
//private function: Stores the keys for later use in the debug print
void node_analysis(bst_node_t *node, int **array, int v_pos, int h_pos, int *max_digits){
    if(node == NULL)
        return;
    array[v_pos][h_pos] = node->key;
    if(digit_val(node->key) > *max_digits)
        *max_digits = digit_val(node->key);
    node_analysis(node->left,array,v_pos+1,h_pos*2,max_digits);
    node_analysis(node->right,array,v_pos+1,h_pos*2+1,max_digits);
}
 
// private function: Prints out graphical BST

void print_bst(int **array, int max_lvl, int max_digits){
    int i,j;
    for (i = 0; i <= max_lvl; i++)
    {
        for (j = 0; j < pow(2,i); j++)
        {
            space_format(max_digits*(pow(2,max_lvl-i)-1));
            if(array[i][j] != -1)  
                print_value(array[i][j],max_digits);
            else
                space_format(max_digits);
            space_format(max_digits*(pow(2,max_lvl-i)));
        }
        printf("\n");
    }
}
 
/* Checks the given tree to assure that it follows the definitions of a BST
 */
void bst_debug_validate(bst_t* tree){
    assert(bst_check(tree->root,INT_MIN,INT_MAX));
}
 
/* Recursively checks the compsition of the given tree to assure it is a 
 * Binary Search tree. Returns boolean of true or false.
 * ##PRIVATE##
 */
int bst_check(bst_node_t* node, int min, int max){
    if(node == NULL)
        return TRUE;
    if(node->key < min || node->key > max)
        return FALSE;
    return bst_check(node->left,min,node->key) && bst_check(node->right,node->key,max);
}
 
/* Recursively searches for the given key from the given node. Returns the
 * data pointer related to the node. Returns NULL if node not found. 
 * ##PRIVATE##
 */
bst_node_t *find_node(bst_node_t* node, bst_key_t key){
    if(node == NULL)
        return NULL;
    if(key == node->key){
        freq_calls++;
        return node;
    }
    else if(key > node->key){
        freq_calls++;
        return find_node(node->right,key);
    }
    else{
        freq_calls++;
        return find_node(node->left,key);
    }
}
 
/* Returns the parent node of the node containing the given key. Also
 * alters the value of the given dir integer to either -1 for the searched
 * node being on the left, or 1 for it being on the right. If parent is
 * not found, dir is not altered and NULL is returned. 
 * ##PRIVATE##
 */
bst_node_t *find_parent(bst_node_t* node, bst_key_t key, int *dir){
    if(node == NULL)
        return NULL;
    if (key < node->key)
    {
        freq_calls++;
        if(node->left == NULL)
            return NULL;
        if(node->left->key == key){
            *dir = -1;
            return node;
        }
        else
            return find_parent(node->left,key,dir);
    }
    else if (key > node->key)
    {
        freq_calls++;
        if(node->right == NULL)
            return NULL;
        if(node->right->key == key){
            *dir = 1;
            return node;
        }
        else
            return find_parent(node->right,key,dir);
    }
    else{ 
        freq_calls++;
        return NULL;}
}
 
/* Returns the data_ptr of a given node.
 * ##PRIVATE##
 */
data_t bst_data(bst_node_t* node){
	if(node==NULL)
		return NULL;
    return node->data_ptr;
}
 
// Returns returns a number associated with the number of children related
int num_child(bst_node_t *node){
    assert(node != NULL);
    int children = 0;
    if(node->left != NULL)
        children += 1;
    if(node->right != NULL)
        children += 2;
    return children;
}
 
// Calculates and returns the number of digits of a given integer to help with formatting the BST print
 int digit_val(int n){
    int digits = 0;
    while(n != 0){
        n /= 10;
        digits++;
    }
    return digits;
 }
 
// Prints r number of space characters to help with formatting the BST print
 void space_format(int n){
    while(n-- != 0) printf(" ");
 }
 
 void print_value(int r, int digits){
    if(digit_val(r) == digits){
        printf("%d",r);
        return;
    }
    int count = 0;
    while(digit_val(r*pow(10,count)) != digits){
        printf("0");
        count++;
    }
    printf("%d",r);
 }

bst_node_t *AVL_insert(bst_t* tree ,bst_node_t* node,int key)
{
    /* 1.  Perform the normal BST rotation */
    if (node == NULL){
    	bst_node_t *node=(bst_node_t*)malloc(sizeof(bst_node_t));
        node->key= key;
        node->left=NULL;
        node->right=NULL;
        node->height=1;
        return(node);
    }
 
    if (key < node->key)
        node->left  = AVL_insert(tree,node->left, key);
    else
        node->right = AVL_insert(tree,node->right, key);
 

    node->height = comp(height_check(node->left), height_check(node->right)) + 1;
 

    int balance = getBalance(node);
 

    // Left Left Case
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);
 
    // Right Right Case
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);
 
    // Left Right Case
    if (balance > 1 && key > node->left->key)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }
 
    // Right Left Case
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

int height_check(bst_node_t *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

bst_node_t *rightRotate(bst_node_t *l)
{
    bst_node_t *r = l->left;
    bst_node_t *T2 = r->right;
 

    r->right = l;
    l->left = T2;
 

    l->height = comp(height_check(l->left), height_check(l->right))+1;
    r->height = comp(height_check(r->left), height_check(r->right))+1;

    return r;
}
 

bst_node_t *leftRotate(bst_node_t *r)
{
    bst_node_t *l = r->right;
    bst_node_t *T2 = l->left;
 

    l->left = r;
    r->right = T2;

    r->height = comp(height_check(r->left), height_check(r->right))+1;
    l->height = comp(height_check(l->left), height_check(l->right))+1;
 
    return l;
}
 
// Get Balance factor of node N
int getBalance(bst_node_t *N)
{
    if (N == NULL)
        return 0;
    return height_check(N->left) - height_check(N->right);
}
int comp(int a, int b)
{
    return (a > b)? a : b;
}
