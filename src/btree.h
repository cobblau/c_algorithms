#ifndef _B_TREE_H
#define _B_TREE_H


#include <common.h>


typedef struct btree_node_s btree_node_t;
typedef struct btree_s      btree_t;


struct btree_node_s {
    uint_t         level;
    uint_t         nactive;
    void         **keys;
    void         **vals;
    btree_node_t **children;
};


struct btree_s {
    btree_node_t  *root;
};


btree_t *btree_create();
int btree_insert(btree_t *bt, void *key, void *val);
int btree_delete(btree_t *bt, void *key);
void *btree_search(btree_t *bt, void *key);
void btree_destroy(btree_t *bt);

#endif
