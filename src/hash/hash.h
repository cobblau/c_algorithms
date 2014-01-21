#ifndef __HASH_H__
#define __HASH_H__

typedef unsigned long hash_value;

typedef int (*HCMP_FUN)(void*, void*);
typedef hash_value (*H_FUN)(void*);
typedef void* (*HALLOC_FUN)(void*);
typedef void (*HFREE_FUN)(void*);

/**
 * This bucket must be placed in top of 
 * every object that uses hashing!!!
 * (Object*) == (Object*) &bucket
 */
typedef struct hash_bucket
{
    struct hash_bucket* next;	/* Next bucket */
    hash_value          hvalue; /* Store hash value for get, rehash */
} hash_bucket;

typedef struct hash_functions
{
    H_FUN               hash;
    HCMP_FUN            cmp;
    HALLOC_FUN          alloc;
    HFREE_FUN           free;
} hash_functions;

typedef struct hash {
    hash_functions      fun;   /* Function block */

    int                 size;
    int                 shrink_water;   /* 20 percent of number of slots */
    int                 expand_water;   /* 80 percent of number of slots */
    int                 ix;             /* Size index in size table */
    int                 used;           /* Number of slots used */
    hash_bucket**       bucket; /* Vector of bucket pointers (objects) */
} hash;

Hash* hash_init(char*, int, hash_functions);

void  hash_delete(hash*);
int   hash_table_sz(hash *);

void* hash_get(hash*, void*);
void* hash_put(hash*, void*);
void* hash_erase(hash*, void*);
void* hash_remove(hash*, void*);
void  hash_foreach(hash*, void (*func)(void *, void *), void *);

void erts_hash_merge(hash* src, hash* dst);

#endif
