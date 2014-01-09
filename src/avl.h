/*
    AVL tree implementation, header file.

    This implementation was written by Kent "ethereal" Williams-King and is
    hereby released into the public domain. Do what you wish with it.
*/

#ifndef AVL_H
#define AVL_H

/* modify this macro to change the prefix */
#define AVL_NAME(name) avl_ ## name
/* memory allocation macros, change as necessary */
#define AVL_ALLOC(variable) variable = malloc(sizeof(*variable))
#define AVL_FREE(variable) free(variable)
#include <stdlib.h> /* for malloc() */

typedef int (*AVL_NAME(comparator_t))(void *key1, void *key2);
typedef void (*AVL_NAME(key_destructor_t))(void *key);

typedef struct AVL_NAME(tree_node_t) {
    struct AVL_NAME(tree_node_t) *left, *right;
    int depth;

    void *key;
    void *data;
} AVL_NAME(tree_node_t);

typedef struct {
    AVL_NAME(tree_node_t) *root;
    AVL_NAME(comparator_t) comparator;
    AVL_NAME(key_destructor_t) destructor;
} AVL_NAME(tree_t);

void AVL_NAME(initialize)(AVL_NAME(tree_t) *tree,
    AVL_NAME(comparator_t) comparator, AVL_NAME(key_destructor_t) destructor);

void *AVL_NAME(search)(AVL_NAME(tree_t) *tree, void *key);
void *AVL_NAME(insert)(AVL_NAME(tree_t) *tree, void *key, void *data);
void *AVL_NAME(remove)(AVL_NAME(tree_t) *tree, void *key);

int AVL_NAME(tree_depth)(AVL_NAME(tree_t) *tree);

#endif
