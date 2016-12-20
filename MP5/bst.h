//bst.h
//Denzel Murdaugh
//MP5
//ECE 2230
 
//Public
enum balanceoptions {BST, AVL, TWOTHREET};
 
typedef void *data_t;
typedef int bst_key_t;
typedef struct bst_node_tag
{
    data_t data_ptr;
    bst_key_t key;
    int height;
    struct bst_node_tag *left;
    struct bst_node_tag *right;
} bst_node_t;
typedef struct bst_tag
{
    bst_node_t *root;
    int tree_policy; //must be balanceoptions
    int tree_size;
    int num_key_recent_comparisons;
} bst_t;
 
//PUBLIC FUNCTIONS
data_t bst_access(bst_t*, bst_key_t);
bst_t *bst_construct(int);
void bst_destruct(bst_t*);
int bst_insert(bst_t*,bst_key_t,data_t);
data_t bst_remove(bst_t*,bst_key_t);
int bst_size(bst_t*);
int bst_stats(bst_t*);
void bst_debug_print_tree(bst_t*);
void bst_debug_validate(bst_t*);
int bst_int_path_len(bst_t *tree);
