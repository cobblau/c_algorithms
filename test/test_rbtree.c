
#include "rbtree.h"


rbtree_t  rbtree;
rbtree_node_t  sentinel;

typedef struct {
    rbtree_node_t  node;
    uint32_t       val;
} test_node_t;


void
test_rbtree_insert_value(rbtree_node_t *temp, rbtree_node_t *node,
                    rbtree_node_t *sentinel)
{
    rbtree_node_t  **p;

    for ( ;; ) {

        if (node->key < temp->key) {

            p = &temp->left;

        } else if (node->key > temp->key) {

            p = &temp->right;

        }

        if (*p == sentinel) {
            break;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left  = sentinel;
    node->right = sentinel;
    rbt_red(node);
}

uint32_t 
rbtree_find(rbtree_t *rbtree, uint32_t key)
{
    test_node_t     *tnode;
    rbtree_node_t   *node, *sentinel;


    node = rbtree->root;

    while (node != sentinel) {
        tnode = (test_node_t *) node;

        if (key < tnode->val) {

            node = node->left;

            continue;
        }

        if (key > tnode->val) {

            node = node->right;

            continue;
        }

        printf("found\n");
        return 0;

    }
    
    printf("not found\n");
    return 0;
}

int main()
{

    test_node_t    n1, n2, n3, n4;

    rbtree_init(&rbtree, &sentinel, test_rbtree_insert_value);

    n1.val = 23;
    n1.node.key = n1.val;
    rbtree_insert(&rbtree, &n1.node);

    n2.val = 11;
    n2.node.key = n2.val;
    rbtree_insert(&rbtree, &n2.node);

    n3.val = 25;
    n3.node.key = n3.val;
    rbtree_insert(&rbtree, &n3.node);

    n4.val = 15;
    n4.node.key = n4.val;
    rbtree_insert(&rbtree, &n4.node);

    rbtree_find(&rbtree,  11);
    rbtree_find(&rbtree,  13);    
    return 0;
}
