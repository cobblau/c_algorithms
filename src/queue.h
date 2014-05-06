#ifndef _QUEUE_H_
#define _QUEUE_H_

/**
 *
 * Double-ended queue.
 *
 * A double ended queue stores a list of pointers in order.  New data
 * can be added and removed from either end of the queue.
 *
 * queue_init: create a new queue
 * queue_destroy: free the queue
 *
 * queue_push_head: add data to head
 * queue_push_tail: add data to tail
 *
 * queue_pop_head: read data from head and dequeue it
 * queue_pop_tail: read data from tail and dequeue it
 *
 * queue_peek_head: examine the head
 * queue_peek_tail: examine the tail
 *
 * queue_empty: check if the queue is empty
 */

#include "common.h"


typedef struct queue_s           queue_t;
typedef struct queue_entry_s     queue_entry_t;
typedef void (*dealloc_f)(void *d);

struct queue_entry_s {
    void          *data;
    queue_entry_t *prev;
    queue_entry_t *next;
};

struct queue_s {
    queue_entry_t *head;
    queue_entry_t *tail;

    dealloc_f      free;
};

queue_t *queue_init(dealloc_f dealloc);
result_t queue_destroy(queue_t *queue);

result_t queue_push_head(queue_t *queue, void *data);
result_t queue_push_tail(queue_t *queue, void *data);

void *queue_pop_head(queue_t *queue);
void * queue_pop_tail(queue_t *queue);

void *queue_peek_head(queue_t *queue);
void * queue_peek_tail(queue_t *queue);

int_t queue_empty(queue_t *queue);

#endif
