#include <stdio.h>
#include <stdint.h>
#include "avl.h"

#define SIZE 20

int cmp(void *k1, void *k2) {
    uint64_t ku1 = (uint64_t)k1;
    uint64_t ku2 = (uint64_t)k2;
    if(ku1 == ku2) return 0;
    else if(ku1 < ku2) return -1;
    return 1;
}


void kd(void *key) {}

int main() {
    int i, r, j;

    avl_tree_t tree;
    avl_initialize(&tree, cmp, kd);

    for(i = 0; i < 1<<SIZE; i ++) {
        avl_insert(&tree, (void *)(long)i, (void *)(long)i);
    }

    r = 0;
    for(j = 0; j < 1<<8; j ++) {
        for(i = 0; i < 1<<SIZE; i ++) {
            r ^= (int)(long)avl_search(&tree, (void *)(long)i);
        }
    }

    for(i = 0; i < 1<<SIZE; i ++) {
        avl_remove(&tree, (void *)(long)i);
    }

    return 0;
}
