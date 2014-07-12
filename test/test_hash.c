#include "common.h"
#include "hash.h"
#include "hash_functions.h"

typedef struct {
    hash_bucket  slot;
    char        *k;
} elem;

hash_value hash_func(void *arg)
{
    elem *e;

    e = (elem *) arg;

    return ELFHash(e->k, strlen(e->k));
}

int hash_cmp(void *l, void *r)
{
    elem *left = (elem *) l;
    elem *right = (elem *) r;

    return strlen(left->k) == strlen(right->k) &&
           strncmp(left->k, right->k, strlen(left->k));
}

void *hash_alloc(void *arg)
{
    elem *e, *n;
    int   len;

    e = (elem *) arg;

    if ((n = malloc(sizeof(elem))) == NULL) {
        perror("malloc error");
        return NULL;
    }

    len = strlen(e->k);
    if ((n->k = malloc(sizeof(char) * (len + 1))) == NULL) {
        perror("malloc error");
        free(n);
        return NULL;
    }
    memset(n->k, 0, len + 1);
    memcpy(n->k, e->k, len);

    return n;
}

void hash_free(void *arg)
{
    elem *e = (elem *) arg;

    free(e);
}
int main()
{
    hash_functions  funcs;
    hash           *h;
    elem            e;

    funcs.hash = hash_func;
    funcs.cmp = hash_cmp;
    funcs.alloc = hash_alloc;
    funcs.free = free;

    if ((h = hash_init(10, funcs)) == NULL) {
        perror("hash init error");
        return 0;
    }

    e.k = "abc";
    assert(hash_get(h, &e) == NULL);

    assert(hash_put(h, &e) != NULL);
    assert(hash_get(h, &e) != NULL);

    e.k = "xyz";
    hash_put(h, &e);
    assert(hash_get(h, &e) != NULL);

    assert(hash_erase(h, &e) != NULL);
    assert(hash_erase(h, &e) == NULL);

    hash_delete(h);

    return 0;
}
