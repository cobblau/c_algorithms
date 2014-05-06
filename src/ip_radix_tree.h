
#ifndef _IP_RADIX_TREE_H_
#define _IP_RADIX_TREE_H_


/**
 * IP radix tree can be used to store ip. And it is widely used to
 *  store ACL.
 *
 * In ip_radix_tree, each leaf means one bit in ip tree.
 * Left means 0, and right means 1.
 *
 */
#include <common.h>


#define IP_RADIX_NULL   (uintptr_t) -1
#define MAX_PAGES       1024 * 16

typedef struct ip_radix_node_s ip_radix_node_t;

struct ip_radix_node_s {
    ip_radix_node_t  *right;
    ip_radix_node_t  *left;
    ip_radix_node_t  *parent;
    uintptr_t         value;
};


typedef struct ip_radix_tree_s {
    ip_radix_node_t  *root;

    ip_radix_node_t  *free;    /* free list of ip_radix_tree */
    char             *start;
    size_t            size;

    /*
     * memory pool.
     * memory management(esp free) will be so easy by using this facility.
     */
    char             *pools[MAX_PAGES];
    size_t            len;
} ip_radix_tree_t;


ip_radix_tree_t *ip_radix_tree_create();
/**
 * Create an empty ip_radix_tree
 *
 * @Return:
 * An ip radix_tree created.
 * NULL if creation failed.
 */

int_t ip_radix_tree_destroy(ip_radix_tree_t *tree);
/**
 * Destroy the ip radix_tree
 *
 * @Return:
 * OK if deletion succeed.
 * ERROR if error occurs while deleting.
 */


int_t ip_radix_tree_insert(ip_radix_tree_t *tree, uint32_t key, uint32_t mask,
    uintptr_t value);
/**
 * Add an ipv4 into ip_radix_tree
 *
 * @Args:
 * key: ip address
 * mask: key's mask
 * value: value of this IP, may be NULL.
 *
 * @Return:
 * OK for success.
 * ERROR for failure.
 */

int_t ip_radix_tree_delete(ip_radix_tree_t *tree, uint32_t key, uint32_t mask);
/**
 * Delete an ipv4 from ip_radix_tree
 *
 * @Args:
 *
 * @Return:
 * OK for success.
 * ERROR for failure.
 */

uintptr_t ip_radix_tree_find(ip_radix_tree_t *tree, uint32_t key);
/**
 * Find an ipv4 from ip_radix_tree
 *

 * @Args:
 *
 * @Return:
 * Value if succeed.
 * NULL if failed.
 */


int_t ip_radix_tree_insert_a6(ip_radix_tree_t *tree, u_char *key, u_char *mask,
          uintptr_t value);
int_t ip_radix_tree_delete_a6(ip_radix_tree_t *tree, u_char *key, u_char *mask);
uintptr_t ip_radix_tree_find_a6(ip_radix_tree_t *tree, u_char *key);


#endif /* _RADIX_TREE_H_INCLUDED_ */
