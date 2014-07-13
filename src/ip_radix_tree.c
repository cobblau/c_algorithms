

#include <ip_radix_tree.h>


static ip_radix_node_t *ip_radix_alloc(ip_radix_tree_t *tree);


ip_radix_tree_t *
ip_radix_tree_create()
{
    uint32_t         key, mask, inc;
    ip_radix_tree_t *tree;

    tree = (ip_radix_tree_t *) mem_malloc(sizeof(ip_radix_tree_t));
    if (tree == NULL) {
        return NULL;
    }

    tree->free  = NULL;
    tree->start = NULL;
    tree->size  = 0;
    memset(tree->pools, 0, sizeof(ip_radix_tree_t *) * MAX_PAGES);
    tree->len = 0;

    tree->root = ip_radix_alloc(tree);
    if (tree->root == NULL) {
        return NULL;
    }

    tree->root->right  = NULL;
    tree->root->left   = NULL;
    tree->root->parent = NULL;
    tree->root->value  = IP_RADIX_NULL;

    return tree;
}


int_t
ip_radix_tree_insert(ip_radix_tree_t *tree, uint32_t key, uint32_t mask,
    uintptr_t value)
{
    uint32_t          bit;
    ip_radix_node_t  *node, *next;

    bit = 0x80000000;

    node = tree->root;
    next = tree->root;

    while (bit & mask) {
        if (key & bit) {
            next = node->right;

        } else {
            next = node->left;
        }

        if (next == NULL) {
            break;
        }

        bit >>= 1;
        node = next;
    }

    if (next) {
        if (node->value != IP_RADIX_NULL) {
            return ERROR;
        }

        node->value = value;
        return OK;
    }

    while (bit & mask) {
        next = ip_radix_alloc(tree);
        if (next == NULL) {
            return ERROR;
        }

        next->right = NULL;
        next->left = NULL;
        next->parent = node;
        next->value = IP_RADIX_NULL;

        if (key & bit) {
            node->right = next;

        } else {
            node->left = next;
        }

        bit >>= 1;
        node = next;
    }

    node->value = value;

    return OK;
}


int_t
ip_radix_tree_delete(ip_radix_tree_t *tree, uint32_t key, uint32_t mask)
{
    uint32_t          bit;
    ip_radix_node_t  *node;

    bit = 0x80000000;
    node = tree->root;

    while (node && (bit & mask)) {
        if (key & bit) {
            node = node->right;

        } else {
            node = node->left;
        }

        bit >>= 1;
    }

    if (node == NULL) {
        return ERROR;
    }

    if (node->right || node->left) {
        if (node->value != IP_RADIX_NULL) {
            node->value = IP_RADIX_NULL;
            return OK;
        }

        return ERROR;
    }

    for ( ;; ) {
        if (node->parent->right == node) {
            node->parent->right = NULL;

        } else {
            node->parent->left = NULL;
        }

        node->right = tree->free;
        tree->free = node;

        node = node->parent;

        if (node->right || node->left) {
            break;
        }

        if (node->value != IP_RADIX_NULL) {
            break;
        }

        if (node->parent == NULL) {
            break;
        }
    }

    return OK;
}


uintptr_t
ip_radix_tree_find(ip_radix_tree_t *tree, uint32_t key)
{
    uint32_t          bit;
    uintptr_t         value;
    ip_radix_node_t  *node;

    bit = 0x80000000;
    value = IP_RADIX_NULL;
    node = tree->root;

    while (node) {
        if (node->value != IP_RADIX_NULL) {
            value = node->value;
        }

        if (key & bit) {
            node = node->right;

        } else {
            node = node->left;
        }

        bit >>= 1;
    }

    return value;
}


int_t
ip_radix_tree_insert_a6(ip_radix_tree_t *tree, u_char *key, u_char *mask,
    uintptr_t value)
{
    u_char             bit;
    uint_t         i;
    ip_radix_node_t  *node, *next;

    i = 0;
    bit = 0x80;

    node = tree->root;
    next = tree->root;

    while (bit & mask[i]) {
        if (key[i] & bit) {
            next = node->right;

        } else {
            next = node->left;
        }

        if (next == NULL) {
            break;
        }

        bit >>= 1;
        node = next;

        if (bit == 0) {
            if (++i == 16) {
                break;
            }

            bit = 0x80;
        }
    }

    if (next) {
        if (node->value != IP_RADIX_NULL) {
            return ERROR;
        }

        node->value = value;
        return OK;
    }

    while (bit & mask[i]) {
        next = ip_radix_alloc(tree);
        if (next == NULL) {
            return ERROR;
        }

        next->right = NULL;
        next->left = NULL;
        next->parent = node;
        next->value = IP_RADIX_NULL;

        if (key[i] & bit) {
            node->right = next;

        } else {
            node->left = next;
        }

        bit >>= 1;
        node = next;

        if (bit == 0) {
            if (++i == 16) {
                break;
            }

            bit = 0x80;
        }
    }

    node->value = value;

    return OK;
}


int_t
ip_radix_tree_delete_a6(ip_radix_tree_t *tree, u_char *key, u_char *mask)
{
    u_char             bit;
    uint_t         i;
    ip_radix_node_t  *node;

    i = 0;
    bit = 0x80;
    node = tree->root;

    while (node && (bit & mask[i])) {
        if (key[i] & bit) {
            node = node->right;

        } else {
            node = node->left;
        }

        bit >>= 1;

        if (bit == 0) {
            if (++i == 16) {
                break;
            }

            bit = 0x80;
        }
    }

    if (node == NULL) {
        return ERROR;
    }

    if (node->right || node->left) {
        if (node->value != IP_RADIX_NULL) {
            node->value = IP_RADIX_NULL;
            return OK;
        }

        return ERROR;
    }

    for ( ;; ) {
        if (node->parent->right == node) {
            node->parent->right = NULL;

        } else {
            node->parent->left = NULL;
        }

        node->right = tree->free;
        tree->free = node;

        node = node->parent;

        if (node->right || node->left) {
            break;
        }

        if (node->value != IP_RADIX_NULL) {
            break;
        }

        if (node->parent == NULL) {
            break;
        }
    }

    return OK;
}


uintptr_t
ip_radix_tree_find_a6(ip_radix_tree_t *tree, u_char *key)
{
    u_char             bit;
    uintptr_t          value;
    uint_t         i;
    ip_radix_node_t  *node;

    i = 0;
    bit = 0x80;
    value = IP_RADIX_NULL;
    node = tree->root;

    while (node) {
        if (node->value != IP_RADIX_NULL) {
            value = node->value;
        }

        if (key[i] & bit) {
            node = node->right;

        } else {
            node = node->left;
        }

        bit >>= 1;

        if (bit == 0) {
            i++;
            bit = 0x80;
        }
    }

    return value;
}


static ip_radix_node_t *
ip_radix_alloc(ip_radix_tree_t *tree)
{
    ip_radix_node_t  *p;

    if (tree->free) {
        p = tree->free;
        tree->free = tree->free->right;
        return p;
    }

    if (tree->size < sizeof(ip_radix_node_t)) {
        tree->start = (char *) mem_calloc(sizeof(char), PAGE_SIZE);
        if (tree->start == NULL) {
            return NULL;
        }

        tree->pools[tree->len++] = tree->start;
        tree->size = PAGE_SIZE;
    }

    p = (ip_radix_node_t *) tree->start;

    tree->start += sizeof(ip_radix_node_t);
    tree->size -= sizeof(ip_radix_node_t);

    return p;
}

int_t
ip_radix_tree_destroy(ip_radix_tree_t *tree)
{
    size_t    i;


    /* free memory pools */
    for (i = 0; i < tree->len; i++) {
        mem_free(tree->pools[i]);
    }

    mem_free(tree);

    return OK;
}
