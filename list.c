/* list.c
 * Christian Walker
 * ECE 223 Fall 2013
 * MP5
 *
 * Purpose:  To create a binary search tree that inserts and remove entries.  
 *           It can access an specific entry.  Calculate number of keys in the 
	     BST, number of key comparisons, and the internal path lenght.
 *
 * Assumptions: bst.h are included.
 *
 * Bugs: None
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

#define BST 1
static int int_level = 0;

/* Allocates a new, empty tree
   Parameter:
	tree_policy: set to BST default, option to reach type of tree
   Return:
	the empty tree
*/ 
bst_t * bst_construct(int tree_policy)
{
    bst_t *R;
    R = (bst_t *)malloc(sizeof(bst_t));
    R->root = NULL;
    R->tree_size = 0;
    R->tree_policy = tree_policy;
    R->num_recent_key_comparisons = 0;

    return R;

}

/* Search the tree for the key that was giving in the parameter
   Parameter:  
	bst_ptr:  BST tree
	key:      the key to be search in the tree
   Return:
	return the data_t block in the key that was founded
	return NULL if the key wasn't found in the tree
*/	    
data_t bst_access(bst_t *bst_ptr, bst_key_t key)
{
    bst_node_t *temp;
    temp = bst_ptr->root;
    int i = 0;

    while(temp != NULL){
	if(key == temp->key){
	    return (temp->data_ptr);
	}
	else if(temp->key < key)
	    temp = temp->right;
	else
	    temp = temp->left;
	i++;
	bst_ptr->num_recent_key_comparisons = (2*i)+1;
    }

    return (NULL);

}
/* Recursive function to free all of the nodes and data_t in the tree
   Parameter:
	N: root node of the tree
   Return: 
	Void function
*/
void grab_destruct(bst_node_t *N)
{
    if (N == NULL) return;
    grab_destruct(N->right);
    grab_destruct(N->left);

    free(N->data_ptr);
    free(N);
}
/* Calls the grab_destruct to free the nodes in the tree
   then free the header block
   Parameter:
	bst_ptr: BST tree
   Return: 
	Void function
*/
void bst_destruct(bst_t *bst_ptr)
{
    grab_destruct(bst_ptr->root);
    free(bst_ptr);

}
/* Add a new key to tree in the create order if the key is not in the tree
   already.  If key is already in the tree than just the data_t block
   Parameter:
	bst_ptr:  BST tree
	key:      key to the added to the tree
	elem_ptr: data_ptr that is in each entry
   Return:
	in: return 0 if an entry is not added because the key is already in the tree
	    return 1 if a new entry is added
*/
int bst_insert(bst_t *bst_ptr , bst_key_t key, data_t *elem_ptr)
{
    int in = 0;
    int i = 0;
    bst_node_t *new_node;
    new_node = malloc(sizeof(bst_node_t));
    new_node->key = key;
    new_node->data_ptr = elem_ptr;
    new_node->left = NULL;
    new_node->right = NULL;
    data_t *equal_data;
    bst_node_t *temp;

    temp = bst_ptr->root;
    if(temp == NULL){
	bst_ptr->root = new_node;
	bst_ptr->tree_size++;
	return 1;
    }

    while(temp != NULL){
        if(new_node->key == temp->key){
	    equal_data = temp->data_ptr;
	    free(equal_data);
	    temp->data_ptr = elem_ptr;
	    free(new_node);
	    in = 0;
	    break;
        }
	else if(new_node->key > temp->key){
	    if(temp->right == NULL){
		temp->right = new_node;
		bst_ptr->tree_size++;
		in = 1;
		break;
	    }
	    else
	        temp = temp->right;
	}
	else {
	    if(temp->left == NULL){
		temp->left = new_node;
		bst_ptr->tree_size++;
		in = 1;
		break;
	    }
	    else
	        temp = temp->left;
	}
	i++;
    }

    bst_ptr->num_recent_key_comparisons = (2*i)+1;
    return in;

}
/* Remove the matching key in the tree
   Parameter:
	bst_ptr: BST tree
	key:     key should remove from the tree
   Return:
	data_ptr: in the key that is being remove and free the key
	return NULL if the key is found in the tree
*/
data_t bst_remove (bst_t *bst_ptr, bst_key_t key)
{
    int i = 0;
    bst_node_t *rove;
    bst_node_t *prev;
    data_t *temp;
    bst_node_t *replace;
    bst_node_t *hold_prev;
    rove = bst_ptr->root;

    /* IF the tree is empty*/
    if(bst_ptr->root == NULL)
	return NULL;
    else if(bst_ptr->root->key < key){
        prev = rove;
        rove = rove->right;
    }
    else {
        prev = rove;
	rove = rove->left;
    }

    /* IF the key is eqaul to the root entry*/
    if(bst_ptr->root->key == key){
	prev = bst_ptr->root;
	if(prev->left == NULL && prev->right == NULL){
	    bst_ptr->root = NULL;
	    temp = prev->data_ptr;
	    bst_ptr->tree_size--;
	    free (prev);
	    return (temp);
	}

	else if(prev->left == NULL){
	    bst_ptr->root = prev->right;
	    prev->right = NULL;
	    temp = prev->data_ptr;
	    free (prev);
	    bst_ptr->tree_size--;
	    return (temp);
	}

	else{
	    rove = bst_ptr->root->left;
            while(rove->right != NULL){
	        prev = rove;
	        rove = rove->right;
	    }
	    if(bst_ptr->root->left == rove){
	        rove->right = prev->right;
	        prev->right = NULL;
	        prev->left = NULL;
	        temp = prev->data_ptr;
	        free (prev);
	        bst_ptr->root = rove;
	        bst_ptr->tree_size--;
	        return(temp);
	    }
	    else{
	        prev->right = rove->left;


	        rove->left = bst_ptr->root->left;
	        rove->right = bst_ptr->root->right;
	        temp = bst_ptr->root->data_ptr;
	        free (bst_ptr->root);
	        bst_ptr->root = rove;
	        bst_ptr->tree_size--;
	        return (temp);
	    }
        }
    }
    while(rove != NULL){
	if(key == rove->key){
	    /* If the key left and right children is NULL*/
	    if(rove->right == NULL && rove->left == NULL){
		if(prev->right == rove){
		    prev->right = NULL;
		}
		else{
		    prev->left = NULL;
		}
		temp = rove->data_ptr;
		free (rove);
		bst_ptr->tree_size--;
		return (temp);
	    }
	    else{
		hold_prev = rove;
		if(rove->left == NULL){
		    if(prev->left == rove){
			prev->left = rove->right;
		    }
		    else{
			prev->right = rove->right;
		    }
		    rove->right = NULL;
		    temp = rove->data_ptr;
		    free(rove);
		    bst_ptr->tree_size--;
		    return(temp);
		}
		replace = rove->left;
		/* find a replacement for the key that is being remove*/
		while(replace->right != NULL){
		    hold_prev = replace;
		    replace = replace->right;
		}
		if(prev->left == rove){
		    prev->left = replace;
		}
		else{
		    prev->right = replace;
		}
		/* Check for the left right child is set NULL*/
		if(rove->left->right == NULL && replace->key < rove->key){
		    rove->left->right = rove->right;
		    rove->right = NULL;
		    rove->left = NULL;
		}
		else{
		    hold_prev->right = replace->left;
		    replace->right = rove->right;
		    replace->left = rove->left;
		    rove->right = NULL;
		    rove->left = NULL;
		}
		temp = rove->data_ptr;
		free (rove);
		bst_ptr->tree_size--;
		return (temp);
	    }

	}
	else if(rove->key < key){
	    prev = rove;
	    rove = rove->right;
	}
	else{
	    prev = rove;
	    rove = rove->left;
	}
	i++;
        /* # of comparsions to find the key*/
	bst_ptr->num_recent_key_comparisons = (2*i)+1;

    }

    return NULL;
}
/* The amount of keys in the tree
   Parameter:
	bst_ptr:  BST tree
   Return:
	number of keys in the tree
*/
int bst_size(bst_t *bst_ptr)
{
   return (bst_ptr->tree_size);
}
/* The number of key comparsions from the recent call
   Parameter: 
	bst_ptr: BST tree
   Return:
	number of key comparsions from the last call
*/
int bst_stats(bst_t *bst_ptr)
{
   return (bst_ptr->num_recent_key_comparisons);
}
/* Recursive fuction to take each path lenght and add it to a total
   to get the internal path lenght of the tree
   Parameter:
	N:         root node of the tree
	level:     path lenght of each path
	max_level: total amount of paths to get the internal path lenght
   Return:
	Void
*/
void find_path_len(bst_node_t *N, int level, int max_level)
{
    if (N == NULL){
	return;
    }
    find_path_len(N->right, level+1, max_level);
    find_path_len(N->left, level+1, max_level);
    int_level = int_level + level;

}
/* Calls the find_path_len to find the internal path lenght
   Parameter:
	bst_ptr: BST tree
   Return:
	internal path lenght
*/
int bst_int_path_len(bst_t *bst_ptr)
{
    int count = 0;
    int max = 0;
    find_path_len(bst_ptr->root, count, max);
    return int_level;

}

/*Function that print all of the keys in the tree
  Parameter:
	N:     root node of the tree
	level: current level that key is on
  Return:
	Voide
*/
void ugly_print(bst_node_t *N, int level)
{
    int i;
    if (N == NULL) return;
    ugly_print(N->right, level+1);

    for (i=0; i<level; i++) printf("     ");
    printf("%5d\n", N->key);
    ugly_print(N->left, level+1);
}
/* Calls the ugly_print to print the tree
   Parameter:
	bst_ptr: BST tree
   Return:
	Void
*/
void bst_debug_print_tree(bst_t *bst_ptr)
{
    int count = 0;

    ugly_print(bst_ptr->root, count);
}
/* Test to see if the tree is valid*/
int bst_debug_validate_rec(bst_node_t *N)
{
    if (N == NULL) return 0;
    if (N->left != NULL)
	assert(N->left->key < N->key);
    if (N->right != NULL)
	assert(N->right->key > N->key);
    assert(N->data_ptr != NULL);
    return 1 + bst_debug_validate_rec(N->left) + bst_debug_validate_rec(N->right);
}
/* Test to see if the tree is valid*/
void bst_debug_validate(bst_t *bst_ptr)
{
    int size;
    size = bst_debug_validate_rec(bst_ptr->root);
    assert(size == bst_ptr->tree_size);
}

