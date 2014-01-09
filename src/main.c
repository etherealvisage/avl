#include <stdio.h>
#include <stdint.h>
#include "avl.h"

int cmp(void *k1, void *k2) {
    uint64_t ku1 = (uint64_t)k1;
    uint64_t ku2 = (uint64_t)k2;
    if(ku1 == ku2) return 0;
    else if(ku1 < ku2) return -1;
    return 1;
}

void kd(void *key) {}

int main() {
    int i, r;

    avl_tree_t tree;
    avl_initialize(&tree, cmp, kd);

    for(i = 0; i < 1<<10; i ++) {
        avl_insert(&tree, (void *)(long)i, (void *)(long)i);
    }

    r = 0;
    for(i = 0; i < 1<<10; i ++) {
        r ^= (int)(long)avl_search(&tree, (void *)(long)i);
    }

    /*printf("max depth: %i\n", avl_tree_depth(&tree));*/

    printf("Search before: %p\n", avl_search(&tree, (void *)42));
    avl_remove(&tree, (void *)42);
    printf("Search after: %p\n", avl_search(&tree, (void *)42));

    printf("depth before: %i\n", avl_tree_depth(&tree));
    for(i = 0; i < 1<<10; i += 3) {
        avl_remove(&tree, (void *)(long)i);
    }
    printf("depth after: %i\n", avl_tree_depth(&tree));

    return 0;
}
