#include "common.h"
#include "hash.h"

/*
** List of sizes (all are primes)
*/
static const int h_size_table[] = {
    2, 5, 11, 23, 47, 97, 197, 397, 797,  /* double upto here */
    1201,   1597,
    2411,   3203,
    4813,   6421,
    9643,   12853,
    19289,  25717,
    51437,
    102877,
    205759,
    411527,
    823117,
    1646237,
    3292489,
    6584983,
    13169977,
    26339969,
    52679969,
    -1
};



/**
 * Returns size of table in bytes. Stored objects not included.
 */
int
hash_table_sz(hash *h)
{
    int i;


    for (i = 0; h->bucket[i]; i++);
    i++;

    return sizeof(hash) + h->size * sizeof(hash_bucket*) + i;
}


/**
 * init a pre allocated or static hash structure
 * and allocate buckets.
 */
hash* hash_init(int size, hash_functions fun)
{
    hash      *h;
    int        sz;
    int        ix = 0;


    while (h_size_table[ix] != -1 && h_size_table[ix] < size)
        ix++;

    if (h_size_table[ix] == -1) {
        printf("ERROR: too large hash table size (%d) \n", size);
        return NULL;
    }

    h = (hash *) malloc(sizeof(hash));
    if (h == NULL) {
        return NULL;
    }

    size = h_size_table[ix];
    sz = size * sizeof(hash_bucket*);

    h->bucket = (hash_bucket **) malloc(sz);
    if (h->bucket == NULL) {
        return NULL;
    }

    h->fun = fun;
    h->size = size;
    h->shrink_water = h->size / 5;
    h->expand_water = (4 * h->size) / 5;
    h->ix = ix;
    h->used = 0;

    return h;
}

/**
 * Delete hash table and all objects
 */
void hash_delete(hash* h)
{
    int           old_size, i;
    hash_bucket  *b, *b_next;


    old_size = h->size;

    for (i = 0; i < old_size; i++) {

        b = h->bucket[i];

        while (b != (hash_bucket*) 0) {

            b_next = b->next;

            h->fun.free((void*) b);
            b = b_next;
        }
    }

    free(h->bucket);
    free(h);
}

/**
 * Rehash all objects
 */
static void rehash(hash* h, int grow)
{
    int            sz, old_size, i, ix;
    hash_bucket**  new_bucket;
    hash_bucket   *b, *b_next;

    old_size = h->size;

    if (grow) {
        if ((h_size_table[h->ix+1]) == -1)
            return;
        h->ix++;
    } else {
        if (h->ix == 0)
            return;
        h->ix--;
    }

    h->size = h_size_table[h->ix];
    h->shrink_water = h->size/5;
    h->expand_water = (4 * h->size) / 5;
    sz = h->size * sizeof(hash_bucket*);

    new_bucket = (hash_bucket **) malloc(sz);
    if (new_bucket == NULL) {
        return;
    }

    memset(new_bucket, 0, sz);

    h->used = 0;

    for (i = 0; i < old_size; i++) {
        b = h->bucket[i];
        while (b != (hash_bucket*) 0) {
            b_next = b->next;
            ix = b->hvalue % h->size;
            if (new_bucket[ix] == NULL)
                h->used++;
            b->next = new_bucket[ix];
            new_bucket[ix] = b;
            b = b_next;
        }
    }

    free(h->bucket);

    h->bucket = new_bucket;
}

/**
 * Find an object in the hash table
 */
void* hash_get(hash* h, void* tmpl)
{
    int          ix;
    hash_value   hval;
    hash_bucket *b;

    hval = h->fun.hash(tmpl);
    ix = hval % h->size;
    b = h->bucket[ix];

    while(b != (hash_bucket*) 0) {
        if ((b->hvalue == hval) && (h->fun.cmp(tmpl, (void*)b) == 0)) {
            return (void*) b;
        }
        b = b->next;
    }

    return (void*) 0;
}

/**
 * Find or insert an object in the hash table
 */
void* hash_put(hash* h, void* tmpl)
{
    int          ix;
    hash_value   hval;
    hash_bucket *b;

    hval = h->fun.hash(tmpl);
    ix = hval % h->size;
    b = h->bucket[ix];

    while(b != (hash_bucket*) 0) {
        if ((b->hvalue == hval) && (h->fun.cmp(tmpl, (void*)b) == 0)) {
            return (void*) b;
        }
        b = b->next;
    }

    b = (hash_bucket*) h->fun.alloc(tmpl);

    if (h->bucket[ix] == NULL) {
        h->used++;
    }

    b->hvalue = hval;
    b->next = h->bucket[ix];
    h->bucket[ix] = b;

    if (h->used > h->expand_water)  /* rehash at 80% */
        rehash(h, 1);

    return (void*) b;
}

static void
hash_insert_entry(hash* h, hash_bucket* entry)
{
    int            ix;
    hash_value     hval;
    hash_bucket   *b;

    hval = entry->hvalue;
    ix = hval % h->size;
    b = h->bucket[ix];

    while (b != (hash_bucket*) 0) {
        if ((b->hvalue == hval) && (h->fun.cmp((void*)entry, (void*)b) == 0)) {
            abort();		/* Should not happen */
        }
        b = b->next;
    }

    if (h->bucket[ix] == NULL)
        h->used++;

    entry->next = h->bucket[ix];
    h->bucket[ix] = entry;

    if (h->used > h->expand_water)  /* rehash at 80% */
        rehash(h, 1);
}


/**
 * Move all entries in src into dst; empty src.
 * Entries in src must not exist in dst.
 */
void
hash_merge(hash* src, hash* dst)
{
    int           limit, i;
    hash_bucket **bucket, *b, *next;

    limit = src->size;
    bucket = src->bucket;
    src->used = 0;

    for (i = 0; i < limit; i++) {
        b = bucket[i];

        bucket[i] = NULL;
        while (b) {
            next = b->next;
            hash_insert_entry(dst, b);
            b = next;
        }
    }
}

/**
 * Erase hash entry return template if erased
 * return 0 if not erased
 */
void* hash_erase(hash* h, void* tmpl)
{
    int             ix;
    hash_value      hval;
    hash_bucket    *b, *prev;

    hval = h->fun.hash(tmpl);
    ix = hval % h->size;
    b = h->bucket[ix];
    prev = NULL;

    while(b != 0) {

        if ((b->hvalue == hval) &&
            (h->fun.cmp(tmpl, (void*)b) == 0))
        {
            if (prev != 0) {
                prev->next = b->next;
            } else {
                h->bucket[ix] = b->next;
            }

            h->fun.free((void*)b);

            if (h->bucket[ix] == NULL) {
                h->used--;
            }

            if (h->used < h->shrink_water) { /* rehash at 20% */
                rehash(h, 0);
            }

            return tmpl;
        }

        prev = b;
        b = b->next;
    }

    return (void*)0;
}

/**
 * Remove hash entry from table return entry if removed
 * return NULL if not removed
 * NOTE: hash_remove() differs from hash_erase() in that
 *       it returns entry (not the template) and does
 *       *not* call the free() callback.
 */
void *
hash_remove(hash *h, void *tmpl)
{
    int             ix;
    hash_value      hval;
    hash_bucket    *b, *prev;


    hval = h->fun.hash(tmpl);
    ix = hval % h->size;
    b = h->bucket[ix];
    prev = NULL;

    while (b) {

        if ((b->hvalue == hval) && (h->fun.cmp(tmpl, (void*)b) == 0)) {

            if (prev) {
                prev->next = b->next;
            } else {
                h->bucket[ix] = b->next;
            }

            if (h->bucket[ix] == NULL) {
                h->used--;
            }

            if (h->used < h->shrink_water) { /* rehash at 20% */
                rehash(h, 0);
            }

            return (void *) b;
        }

        prev = b;
        b = b->next;
    }

    return NULL;
}

void hash_foreach(hash* h, void (*func)(void *, void *), void *func_arg2)
{
    int          i;
    hash_bucket *b;

    for (i = 0; i < h->size; i++) {
        b = h->bucket[i];
        while(b != (hash_bucket*) 0) {
            (*func)((void *) b, func_arg2);
            b = b->next;
        }
    }
}

