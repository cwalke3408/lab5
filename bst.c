/* tree.c
 * Christian Walker
 * ECE 223 Spring 2014
 * MP5
 
 *Purpose: Insert keys into a sorted BST tree
 
 *Bugs: NONE
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bst.h"

void int_path_len(bst_node_t *, int);
void other_destruct(bst_node_t *); 
bst_node_t * bst_found_remove(bst_node_t *);
void ugly_print(bst_node_t *, int);
int bst_debug_validate_rec(bst_node_t *);


static int Internal_Num = 0;

/*  Create a new tree
    Parameter:
	tree_policy: type of tree to be constructed (BST,AVL,TWOTHREE)
    Return a empty tree
	
*/
bst_t * bst_construct (int tree_policy){
    bst_t *T = NULL;
    T = (bst_t *)malloc(sizeof(bst_t));
    T->root = NULL;
    T->tree_size = 0;
    T->tree_policy = tree_policy;
    T->num_recent_key_comparisons = 0;

    return T;
}
/* Free the header block after calling other_destruct that frees all the
   node in the tree.
   Parameter:
	The tree to be destruct

*/
void bst_destruct(bst_t * bst_ptr){
    other_destruct(bst_ptr->root);
    free(bst_ptr);
}
/* Recursive fuction to reach all the nodes in the tree to free
   bst_node_t and data_ptr.
   Parameter: 
        N: bst_node_t to be freed with its data_pt

*/
void other_destruct(bst_node_t *N){
    if (N == NULL) return;
    other_destruct(N->right);
    other_destruct(N->left);

    free(N->data_ptr);
    free(N);
}

/* Search the tree for the key and return the data block if it found the key
   Parameter: 
	bst_ptr: The tree to be search
	key:     The key to be found in the tree
   Return:
	return the data_t block of the key found in the tree
	return if the key wasn't found in the tree

*/
data_t bst_access(bst_t *bst_ptr, bst_key_t key){

    bst_node_t *rover;
    int i = 0;

    rover = bst_ptr->root;
    while(rover != NULL){
        i++;
        bst_ptr->num_recent_key_comparisons = (2*i)+1;
        
	    if(rover->key == key){
//printf("Access key: %d \n", rover->key);
	        return rover->data_ptr;
	    }
	    else if(rover->key > key)
	        rover = rover->left;

	    else
	        rover = rover->right;
    }
    
    bst_ptr->num_recent_key_comparisons = (2*i)+1;
    return NULL;
}
/* Add the key into a BST tree.  If the key is already in the tree then 
   replace the data_t block in the tree with the new data_t block.
   Parameter:
	bst_ptr:  tree to inserted to 
	key:      the number to be used to sort the tree
	elem_ptr: data_ptr that is store in the bst_node_t.
   Return:
	return 0 if there was an duplicate key and a new key wasn't added
	return 1 if new key added to the tree

*/
int bst_insert(bst_t *bst_ptr, bst_key_t key, data_t elem_ptr){

    bst_node_t *new_node;
    bst_node_t *rover;
    bst_node_t *prev;
    new_node = (bst_node_t *)malloc(sizeof(bst_node_t));
    data_t *temp;
    int leftright = 0;
    int i = 0;

	// tree empty
    if(bst_ptr->root == NULL){
    	bst_ptr->root = new_node;
	    new_node->right = NULL;
	    new_node->left = NULL;
	    new_node->key = key;
	    new_node->data_ptr = elem_ptr;
	    bst_ptr->tree_size = 1;
	    return 1;
    }

    rover = bst_ptr->root;

    do{
	    if(key == rover->key){
	        temp = rover->data_ptr;
	        rover->data_ptr = elem_ptr;
	        free(temp);
	        free(new_node);
	        return 0;
	    }
	    else if(key > rover->key){
	        prev = rover;
	        rover = rover->right;
	        leftright = 1;
	    }
	    else{
	        prev = rover;
	        rover = rover->left;
	        leftright = 0;
	    }
	i++;

    }while(rover != NULL);

    if(leftright == 1){
	    prev->right = new_node;
	    new_node->key = key;
	    new_node->data_ptr = elem_ptr;
	    new_node->right = NULL;
	    new_node->left = NULL;
	    bst_ptr->tree_size++;
    }
    else{
	    prev->left = new_node;
	    new_node->key = key;
	    new_node->data_ptr = elem_ptr;
	    new_node->right = NULL;
	    new_node->left = NULL;
	    bst_ptr->tree_size++;
    }

    bst_ptr->num_recent_key_comparisons = (2*i)+1;
    return 1;
}
/* Function to help the bst_remove function by finding a replacement node to replace
   the node being removed
   Paramter: 
	rover: to node to be remove and replace
   Return:
	the node that will replace the node being removed

*/
bst_node_t * bst_found_remove(bst_node_t *rover){

    bst_node_t *rove_replace;
    bst_node_t *prev = NULL;

    rove_replace = rover->left;
    if(rove_replace == NULL){
	    return rover->right;
    }
    while(rove_replace->right != NULL){
	    prev = rove_replace;
	    rove_replace = rove_replace->right;
    }
    if(prev != NULL){
        prev->right = rove_replace->left;
        rove_replace->right = rover->right;
        rove_replace->left = rover->left;
    }
    else{
        rove_replace->right = rover->right;
    }
    return (rove_replace);

}

/* Locate the key to be remove from the tree and free the node
   Parameter:
	bst_ptr: the tree to be removing
	key:     the key to found in the tree to be remove
   Return:
	the data_t block from the bst_node_t that was remove from the list

*/
data_t bst_remove(bst_t * bst_ptr, bst_key_t key){

    bst_node_t *rover;
    bst_node_t *prev = NULL;
    bst_node_t *replace;
    data_t *temp;
    int i = 0;

    rover = bst_ptr->root;
    if(rover == NULL)
	    return NULL;

    do{
        i++;
	    if(key == rover->key){
	        replace = bst_found_remove(rover);
	        if(prev == NULL)
		        bst_ptr->root = replace;
	        else if(prev->right == rover){
		        prev->right = replace;
	        }
	        else{
		        prev->left = replace;
	        }

            bst_ptr->tree_size--;
	        temp = rover->data_ptr;
	        free(rover);
	        return (temp);
	    }
	    else if(key > rover->key){
	        prev = rover;
	        rover = rover->right;
	    }
	    else{
	        prev = rover;
	        rover = rover->left;
	    }
    }while(rover != NULL);
//bst_debug_validate(bst_ptr);

    bst_ptr->num_recent_key_comparisons = (2*i)+1;

    return NULL;
}

/* 
    Return the size of the list of the tree
*/
int bst_size(bst_t *bst_ptr){
//printf("TSize: %d \n", bst_ptr->tree_size);
    return bst_ptr->tree_size;
}

/*
    Return the number of recent key comparisons from the last time
    it called bst_remove, bst_insert, or bst_access.
*/
int bst_stats(bst_t *bst_ptr){
    return bst_ptr->num_recent_key_comparisons;
}
/* Recursive function that run that the tree to find the internal path lenght
   Parameter: 
	N:     The node to check what lvl its or if its equal to NULL
	count: the value of what level the node is on to be added to the 
	       Internal_Num total

*/
void int_path_len(bst_node_t *N, int count){
    if(N == NULL) return;
    count++;
    int_path_len(N->right, count);
    int_path_len(N->left, count);
    Internal_Num += count;
}
/* Call the int_path_len to find the internal path lenght of the tree
   and return the value
*/
int bst_int_path_len(bst_t *bst_ptr){

    Internal_Num = 0;
    int count = -1;
    int_path_len(bst_ptr->root, count);
    return Internal_Num;
    
}
/*
    The function to call ugly_print to print the tree
*/
void bst_debug_print_tree(bst_t *T){
    int level = 0;
    ugly_print(T->root, level);
}

/* Recursive function that prints out keys of the tree
   Paramter: 
	N:     node to be printed
	level: the level that the node is on
*/
void ugly_print(bst_node_t *N, int level) {
    int i;
    if (N == NULL) return;
    ugly_print(N->right, level+1);
    for(i = 0; i<level; i++){
	 printf("     ");
    }
    printf("%5d\n", N->key);
    ugly_print(N->left, level+1);
}
/*
    Calls bst_debug_validate_rec to validate the tree
*/
void bst_debug_validate(bst_t *T)
{
    int size;
    size = bst_debug_validate_rec(T->root);
    assert(size == T->tree_size);

}
/* 
    Recursive function that run asserts on the tree to see if its correct
*/
int bst_debug_validate_rec(bst_node_t *N)
{
    if (N == NULL) return 0;
    if (N->left != NULL)
        /* assert that left key is less than N's key */
        assert(N->left->key < N->key);
    if (N->right != NULL)
        /* assert that right  key is greater than N's key */
        assert(N->right->key > N->key);
    assert(N->data_ptr != NULL);
    return 1 + bst_debug_validate_rec(N->left) +
        bst_debug_validate_rec(N->right);
}


