#ifndef _SET_H_
#define _SET_H_

/**
 *
 * Set of elements.
 *
 * Set stores a collection of values. Each value can only exist once in Set.
 *
 * set_init: create a new set
 * set_destroy: free the set
 *
 * set_insert : add a element to set
 * set_remove : remove the element from set
 * set_find   : find the specified element int a set
 * set_foreach: iterate over all elements in a set
 * set_num_entries: get the number of entries in a set
 *
 * set_union : perform a union of two sets.
 * set_intersection: perform an intersection of two sets.
 *
 * Note: Actually, set is a wrap of hash_table.
 */

#include "common.h"

typedef struct set_bucket_s set_bucket_t;
typedef struct set_s        set_t;

struct set_bucket_s {
    void         *data;
    set_bucket_t *next;
};

struct set_s {
    int             entries;    /* number of elements in set */
    int             index;      /* index in prime table */
    int             size;       /* bucket's size */
    set_bucket_t  **bucket;
    set_hash_func   hash_func;
    set_equal_func  equal_func;
    set_free_func   free_func;
};


typedef unsigned long (*set_hash_func)(void *data);
/**
 * Hash function. Generate a hash key for data to be stored in a set.
 */

typedef int (*set_equal_func)(void *data1, void *data2);
/**
 * Equality function. compares two values to determine if they are quivalent.
 */

typedef void (*set_free_func)(void *data);
/**
 * Free function. Free the data stored in a set.
 */

typedef void *(set_copy_func)(void *data);
/**
 * Given a pointer of data. return a copy of data.
 */

set_t *set_init(set_hash_func hfunc, set_equal_func eqfunc, set_free_func ffunc);

void set_destroy(set_t *set);

#define set_num_entries(s)  (s)->entries

result_t set_insert(set_t *set, void *data);
/**
 * Add a value to set.
 *
 * @Returns:
 *     OK if success;
 *     ERROR if error occurs.
 */

result_t set_remove(set_t *set, void *data);
/**
 * Remove data from set.
 *
 * @Returns:
 *     OK if success;
 *     ERROR if data was not exist in set.
 */

result_t set_find(set_t *set, void *data);
/**
 * Remove data from set.
 *
 * @Returns:
 *     OK if data existed;
 *     ERROR if data was not exist in set.
 */

set_t *set_union(set_t *set1, set_t *set2, set_copy_func cfunc);
/**
 * Perform a union of two sets.
 *
 * @Args:
 *  cfunc: function used to copy data. If cfunc is NULL, no copying is performed
 *         and the reference of data is added into union set.
 *
 * @Returns:
 *   A new set contains all data which are in both sets.
 */

set_t *set_intersection(set_t *set1, set_t *set2, set_copy_func cfunc);
/**
 * Perform a union of two sets.
 *
 * @Args:
 *  cfunc: function used to copy data. If cfunc is NULL, no copying is performed
 *         and the reference of data is added into union set.
 *
 * @Returns:
 *   A new set contains all data which are in both sets.
 */

#endif
