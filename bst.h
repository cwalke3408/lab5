
/*  Christian Waler
 *  ECE 223 Fall 2013
 *  bst.h
 *
 */

typedef void *data_t;
typedef int bst_key_t;
//typedef int BST, AVL, TWOTHREE;

typedef struct bst_key_tag {
    data_t data_ptr;
    bst_key_t key;
    struct bst_key_tag *left;
    struct bst_key_tag *right;
} bst_node_t;

typedef struct bst_tag {
    bst_node_t *root;
    int tree_size;
    int tree_policy;
    int num_recent_key_comparisons;
} bst_t;
/*

*/
typedef bst_key_t *Iterator;
typedef bst_t *Root;

data_t bst_access(bst_t *bst_ptr, bst_key_t key);
bst_t *bst_construct(int tree_policy);
void bst_destruct(bst_t *bst_ptr);
int bst_insert(bst_t *bst_ptr, bst_key_t key, data_t elem_ptr);
data_t bst_remove(bst_t *bst_ptr, bst_key_t key);
int bst_size(bst_t *bst_ptr);
int bst_stats(bst_t *bst_ptr);
int bst_int_path_len(bst_t *bst_ptr);

void bst_debug_print_tree(bst_t *bst_ptr);
void bst_debug_validate(bst_t *bst_ptr);
int bst_debug_validate_rec(bst_node_t *N);
void ugly_print(bst_node_t *N, int level);
void find_path_len(bst_node_t *N, int level, int max_level);
void grab_destruct(bst_node_t *N);
