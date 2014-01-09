#include "avl.h"
/* required definitions */
#ifndef NULL
    #define NULL ((void *)0)
#endif

static void *AVL_NAME(search_helper)(AVL_NAME(tree_t) *tree,
    AVL_NAME(tree_node_t) *node, void *key);
static void *AVL_NAME(insert_helper)(AVL_NAME(tree_t) *tree,
    AVL_NAME(tree_node_t) **node, void *key, void *data);
static void *AVL_NAME(remove_helper)(AVL_NAME(tree_t) *tree,
    AVL_NAME(tree_node_t) **node, void *key);
static void AVL_NAME(remove_depth_helper)(AVL_NAME(tree_node_t) *ptr);

#define AVL_LEFT 0
#define AVL_RIGHT 1
/* rotates a node and its left/right child as appropriate (left=0, right=1) */
static void AVL_NAME(rotate)(AVL_NAME(tree_node_t) **ptr, int which);

static void AVL_NAME(rebalance)(AVL_NAME(tree_node_t) **ptr);
static int AVL_NAME(balance_factor)(AVL_NAME(tree_node_t) *ptr);
static void AVL_NAME(update_depth)(AVL_NAME(tree_node_t) *ptr);

void AVL_NAME(initialize)(AVL_NAME(tree_t) *tree,
    AVL_NAME(comparator_t) comparator, AVL_NAME(key_destructor_t) destructor) {
    
    tree->comparator = comparator;
    tree->destructor = destructor;
    tree->root = NULL;
}

void *AVL_NAME(search)(AVL_NAME(tree_t) *tree, void *key) {
    return AVL_NAME(search_helper)(tree, tree->root, key);
}

static void *AVL_NAME(search_helper)(AVL_NAME(tree_t) *tree,
    AVL_NAME(tree_node_t) *node, void *key) {

    int cmp;

    if(!node) return NULL;

    cmp = tree->comparator(key, node->key);
    if(cmp == 0) return node->data;
    else if(cmp < 0) {
        return AVL_NAME(search_helper)(tree, node->left, key);
    }
    else /* if(cmp > 0) */ {
        return AVL_NAME(search_helper)(tree, node->right, key);
    }
}

void *AVL_NAME(insert)(AVL_NAME(tree_t) *tree, void *key, void *data) {
    return AVL_NAME(insert_helper)(tree, &tree->root, key, data);
}

static void *AVL_NAME(insert_helper)(AVL_NAME(tree_t) *tree,
    AVL_NAME(tree_node_t) **node, void *key, void *data) {

    int cmp;
    void *ret;

    if(!*node) {
        AVL_ALLOC(*node);
        (*node)->depth = 1;
        (*node)->key = key;
        (*node)->data = data;
        (*node)->left = (*node)->right = NULL;

        return NULL;
    }

    cmp = tree->comparator(key, (*node)->key);
    if(cmp == 0) {
        void *old = (*node)->data;
        (*node)->data = data;
        return old;
    }
    else if(cmp < 0) {
        ret = AVL_NAME(insert_helper)(tree, &(*node)->left, key, data);
    }
    else /*if(cmp > 0)*/ {
        ret = AVL_NAME(insert_helper)(tree, &(*node)->right, key, data);
    }

    AVL_NAME(rebalance)(node);

    AVL_NAME(update_depth)(*node);

    return ret;
}

void *AVL_NAME(remove)(AVL_NAME(tree_t) *tree, void *key) {
    return AVL_NAME(remove_helper)(tree, &tree->root, key);
}

static void *AVL_NAME(remove_helper)(AVL_NAME(tree_t) *tree,
    AVL_NAME(tree_node_t) **node, void *key) {

    int cmp;
    void *ret;

    if(!*node) return NULL;

    cmp = tree->comparator(key, (*node)->key);

    if(cmp < 0) ret = AVL_NAME(remove_helper)(tree, &(*node)->left, key);
    else if(cmp > 0) ret = AVL_NAME(remove_helper)(tree, &(*node)->right, key);
    else /* if(cmp == 0) */{
        AVL_NAME(tree_node_t) **y, *p = NULL;

        ret = (*node)->data;
        if(tree->destructor) tree->destructor((*node)->key);

        /* complicated case */
        if((*node)->left && (*node)->right) {
            y = &(*node)->left;
            while((*y)->right) y = &(*y)->right;

            /* copy contents out */
            (*node)->key = (*y)->key;
            (*node)->data = (*y)->data;

            p = (*y)->left;
            AVL_FREE(*y);
            *y = p;

            /* TODO: need to update depths along path */
            AVL_NAME(remove_depth_helper)((*node)->left);
        }
        else if((*node)->left) {
            p = (*node)->left;
            AVL_FREE(*node);
            *node = p;
        }
        else if((*node)->right) {
            p = (*node)->right;
            AVL_FREE(*node);
            *node = p;
        }
        else {
            AVL_FREE(*node);
            *node = NULL;
        }
    }

    if(*node) {
        AVL_NAME(update_depth)(*node);
        AVL_NAME(rebalance)(node);
    }

    return ret;
}

static void AVL_NAME(remove_depth_helper)(AVL_NAME(tree_node_t) *ptr) {
    if(ptr) {
        AVL_NAME(remove_depth_helper)(ptr->right);
        AVL_NAME(update_depth)(ptr);
    }
}

static void AVL_NAME(rebalance)(AVL_NAME(tree_node_t) **node) {
    int delta = AVL_NAME(balance_factor)(*node);

    /* two rotation directions */
    if(delta == 2) {
        if(AVL_NAME(balance_factor((*node)->left) < 0)) {
            AVL_NAME(rotate)(&(*node)->left, AVL_LEFT);
        }
        AVL_NAME(rotate)(node, AVL_RIGHT);
    }
    else if(delta == -2) {
        if(AVL_NAME(balance_factor((*node)->right) > 0)) {
            AVL_NAME(rotate)(&(*node)->right, AVL_RIGHT);
        }
        AVL_NAME(rotate)(node, AVL_LEFT);
    }
}

static void AVL_NAME(rotate)(AVL_NAME(tree_node_t) **node, int dir) {
    AVL_NAME(tree_node_t) *ch;
    
    if(dir == 0) {
        ch = (*node)->right;
        
        (*node)->right = (*node)->right->left;
        ch->left = *node;
        AVL_NAME(update_depth)(*node);
        *node = ch;
    }
    else {
        ch = (*node)->left;
        
        (*node)->left = (*node)->left->right;
        ch->right = *node;
        AVL_NAME(update_depth)(*node);
        *node = ch;
    }
    AVL_NAME(update_depth)(*node);
}

static int AVL_NAME(balance_factor)(AVL_NAME(tree_node_t) *ptr) {
    int delta = 0;
    if(ptr->left) delta = ptr->left->depth;
    if(ptr->right) delta -= ptr->right->depth;
    return delta;
}

static void AVL_NAME(update_depth)(AVL_NAME(tree_node_t) *ptr) {
    ptr->depth = 1;
    if(ptr->left) ptr->depth = ptr->left->depth;
    if(ptr->right && ptr->depth < ptr->right->depth) {
        ptr->depth = ptr->right->depth;
    }
    ptr->depth ++;
}

int AVL_NAME(tree_depth)(AVL_NAME(tree_t) *tree) {
    if(tree->root) return tree->root->depth;
    return 0;
}
