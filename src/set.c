
#include <set.h>

static unsigned int set_primes[] = {
    251, 383, 571, 863, 1291, 1933, 2909, 4373, 6553, 9839, 14759, 22133,
    33211, 49811, 74719, 112069, 168127, 252193, 378289, 567407, 851131,
    1276721, 1915057, 2872621, 4308937, 6463399, 9695099, 14542651,
    21813997, 32721001, 49081441, 73622251, 11043383, 165650033,
    248475107, 372712667, 559068997, 838603499, 1257905249, 1886857859,
};

static int set_primes_num = sizeof(set_primes) / sizeof(unsigned int);


static void set_rehash(set_t *set, int grow /*1: enlarge, 0: shrink */)
{
    int           old_size, old_index, i, index;
    set_entry_t **old_bucket, *entry, *next;

    old_size = set->size;
    old_bucket = set->bucket;

    if (grow) {
        /* enlarge */
        if (set->index < set_primes_num) {
            set->index++;
            set->size = set_primes[set->index];
        } else {
            return;
        }
    } else {
        /* shrink */
        if (set->index > 0) {
            set->index--;
            set->size = set_primes[set->index];
        } else {
            return;
        }
    }

    set->bucket = mem_calloc(set->size, sizeof(set_entry_t *));
    if (set->bucket == NULL) {
        set->size = old_size;
        set->index = old_index;
        set->bucket = old_bucket;
        return;
    }

    for (i = 0; i < old_size; i++) {

        entry = old_bucket[i];

        while(entry != (set_entry_t *) 0) {
            next = entry->next;

            index = set->hash_func(entry->data) % set->size;
            entry->next = set->bucket[index];
            set->bucket[index] = entry;

            entry = next;
        }
    }

    mem_free(old_bucket);
}


set_t *
set_init(set_hash_func hfunc, set_equal_func eqfunc, set_free_func ffunc)
{
    set_t *s;

    s = (set_t *) mem_alloc(sizeof(set_t));
    if (s == NULL) {
        return NULL;
    }

    s->entries = 0;
    s->index = 0;
    s->size = set_primes[s->index];
    s->bucket = mem_calloc(s->size, sizeof(set_entry_t *));
    if (s->bucket == NULL) {
        mem_free(s);
        return NULL;
    }

    s->hash_func = hfunc;
    s->equal_func = eqfunc;
    s->free_func = ffunc;

    return s;
}

void
set_destroy(set_t *set)
{
    int          i;
    set_entry_t *entry, *next;

    for (i = 0; i < set->size; i++) {
        entry = set->bucket[i];

        while (entry != (set_entry_t *) 0) {
            next = entry->next;

            set->free_func(entry->data);
            entry = next;
        }
    }

    mem_free(set->bucket);
    mem_free(set);
}


result_t
set_insert(set_t *set, void *data)
{
    int    index;
    set_entry_t *entry;


    index = set->hash_func(data) % set->size;

    entry = set->bucket[index];

    while (entry != (set_entry_t *) 0) {

        if (set->equal_func(data, entry->data) == OK) {
            return OK;
        }

        entry = entry->next;
    }

    entry = (set_entry_t *) mem_alloc(sizeof(set_entry_t));
    if (entry == NULL) {
        return ERROR;
    }

    entry->data = data;

    entry->next = set->bucket[index];
    set->bucket[index] = entry;

    set->entries++;

    if (set->entries > set->size * 4 / 5) {
        set_rehash(set, 1);
    }

    return OK;
}


result_t
set_remove(set_t *set, void *data)
{
    int          index;
    set_entry_t *entry, *prev = NULL;


    index = set->hash_func(data) % set->size;

    entry = set->bucket[index];

    while (entry != (set_entry_t *) 0) {

        if (set->equal_func(data, entry->data) == OK) {

            if (prev) {
                prev->next = entry->next;
            } else {
                set->bucket[index] = entry->next;
            }

            set->free_func(entry->data);
            mem_free(entry);

            set->entries--;

            if (set->entries < set->size * 1 / 5) {
                set_rehash(set, 0);
            }

            return OK;
        }

        prev = entry;
        entry = entry->next;
    }


    return ERROR;
}


result_t
set_find(set_t *set, void *data)
{
    int          index;
    set_entry_t *entry;


    index = set->hash_func(data) % set->size;

    entry = set->bucket[index];

    while (entry != (set_entry_t *) 0) {

        if (set->equal_func(data, entry->data) == OK) {

            return OK;
        }

        entry = entry->next;
    }


    return ERROR;
}


void
set_foreach(set_t *set, void (*func)(void *data, void *args), void* args)
{
    int          i;
    set_entry_t *entry;

    for (i = 0; i < set->size; i++) {
        entry = set->bucket[i];

        while(entry != (set_entry_t *) 0) {

            func(entry->data, args);

            entry = entry->next;
        }
    }
}


set_t *
set_union(set_t *set1, set_t *set2, set_copy_func cfunc)
{
    int          i;
    set_t       *new;
    set_entry_t *entry;
    void        *data;
    result_t     ret;


    new = set_init(set1->hash_func, set1->equal_func, set1->free_func);
    if (new == NULL) {
        return NULL;
    }

    for (i = 0; i < set1->size; i++) {
        entry = set1->bucket[i];

        while(entry != (set_entry_t *) 0) {

            if (cfunc) {
                data = cfunc(entry->data);
            } else {
                data = entry->data;
            }

            ret = set_insert(new, data);
            if (ret == ERROR) {
                goto failure;
            }

            entry = entry->next;
        }
    }

    for (i = 0; i < set2->size; i++) {
        entry = set2->bucket[i];

        while(entry != (set_entry_t *) 0) {

            if (set_find(set1, entry->data) == ERROR) {

                if (cfunc) {
                    data = cfunc(entry->data);
                } else {
                    data = entry->data;
                }

                ret = set_insert(new, data);
                if (ret == ERROR) {
                    goto failure;
                }
            }
            entry = entry->next;
        }
    }

    return new;

 failure:
    if (cfunc) {
        set_destroy(new);
    } else {
        mem_free(new->bucket);
        mem_free(new);
    }

    return NULL;
}


set_t *
set_intersection(set_t *set1, set_t *set2, set_copy_func cfunc)
{
    int          i;
    set_t       *new;
    set_entry_t *entry;
    void        *data;
    result_t     ret;


    new = set_init(set1->hash_func, set1->equal_func, set1->free_func);
    if (new == NULL) {
        return NULL;
    }

    for (i = 0; i < set1->size; i++) {
        entry = set1->bucket[i];

        while(entry != (set_entry_t *) 0) {

            if (set_find(set2, entry->data) == OK) {

                if (cfunc) {
                    data = cfunc(entry->data);
                } else {
                    data = entry->data;
                }

                ret = set_insert(new, data);
                if (ret == ERROR) {
                    goto failure;
                }
            }

            entry = entry->next;
        }
    }

    return new;

 failure:
    if (cfunc) {
        set_destroy(new);
    } else {
        mem_free(new->bucket);
        mem_free(new);
    }

    return NULL;
}
