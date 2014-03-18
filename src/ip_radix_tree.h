
#ifndef _IP_RADIX_TREE_H_INCLUDED_
#define _IP_RADIX_TREE_H_INCLUDED_


#include <common.h>


#define IP_RADIX_NULL   (uintptr_t) -1

typedef struct ip_radix_node_s {
    ip_radix_node_t  *right;
    ip_radix_node_t  *left;
    ip_radix_node_t  *parent;
    uintptr_t         value;
} ip_radix_node_t;


typedef struct ip_radix_tree_s {
    ip_radix_node_t  *root;
    ip_radix_node_t  *free;    /* free list of ip_ip_radix_tree */
    char             *start;
    size_t            size;
} ip_radix_tree_t;


ip_radix_tree_t *ip_radix_tree_create();
int_t ip_radix_tree_destroy(ip_radix_tree_t *tree);

/* for ipv4 */
int_t ip_radix32tree_insert(ip_radix_tree_t *tree, uint32_t key, uint32_t mask,
          uintptr_t value);
int_t ip_radix32tree_delete(ip_radix_tree_t *tree, uint32_t key, uint32_t mask);
uintptr_t ip_radix32tree_find(ip_radix_tree_t *tree, uint32_t key);

/* for ipv6 */
int_t ip_radix128tree_insert(ip_radix_tree_t *tree, u_char *key, u_char *mask,
          uintptr_t value);
int_t ip_radix128tree_delete(ip_radix_tree_t *tree, u_char *key, u_char *mask);
uintptr_t ip_radix128tree_find(ip_radix_tree_t *tree, u_char *key);


#endif /* _RADIX_TREE_H_INCLUDED_ */
