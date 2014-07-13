#include <queue.h>


queue_t *
queue_init(dealloc_f dealloc)
{
    queue_t *q;


    if((q = (queue_t*) mem_alloc(sizeof(queue_t))) == NULL) {
        return NULL;
    }

    q->head = NULL;
    q->tail = NULL;
    q->free = dealloc;

    return q;
}

result_t
queue_destroy(queue_t *queue)
{
    void *data;


    while (!queue_empty(queue)) {

        data = queue_pop_head(queue);
        queue->free(data);
    }

    mem_free(queue);
}

result_t
queue_push_head(queue_t *queue, void *data)
{
    queue_entry_t  *ne;


    /* Create the new entry and fill in the fields in the structure */

    ne = (queue_entry_t *) mem_alloc(sizeof(queue_entry_t));
    ne->data = data;
    ne->prev = NULL;
    ne->next = queue->head;

    /* Insert into the queue */

    if (queue->head == NULL) {

        /* If the queue was previously empty, both the head and tail must
         * be pointed at the new entry */

        queue->head = ne;
        queue->tail = ne;

    } else {

        /* First entry in the list must have prev pointed back to this
         * new entry */

        queue->head->prev = ne;

        /* Only the head must be pointed at the new entry */

        queue->head = ne;
    }
}

result_t
queue_push_tail(queue_t *queue, void *data)
{
    queue_entry_t *ne;


    /* Create the new entry and fill in the fields in the structure */

    ne = (queue_entry_t *) mem_alloc(sizeof(queue_entry_t));
    ne->data = data;
    ne->prev = queue->tail;
    ne->next = NULL;

    /* Insert into the queue tail */

    if (queue->tail == NULL) {

        /* If the queue was previously empty, both the head and tail must
         * be pointed at the new entry */

        queue->head = ne;
        queue->tail = ne;

    } else {

        /* The current entry at the tail must have next pointed to this
         * new entry */

        queue->tail->next = ne;

        /* Only the tail must be pointed at the new entry */

        queue->tail = ne;
    }
}

void *
queue_pop_head(queue_t *queue)
{
    queue_entry_t *entry;
    void          *res;


    /* Check the queue is not empty */
    if (queue_empty(queue)) {
        return NULL;
    }

    /* Unlink the first entry from the head of the queue */
    entry = queue->head;
    queue->head = entry->next;
    res = entry->data;

    if (queue->head == NULL) {

        /* If doing this has unlinked the last entry in the queue, set
         * tail to NULL as well. */

        queue->tail = NULL;
    } else {

        /* The new first in the queue has no previous entry */

        queue->head->prev = NULL;
    }

    /* Free back the queue entry structure */

    queue->free(entry);

    return res;
}

void *
queue_pop_tail(queue_t *queue)
{
    queue_entry_t  *entry;
    void           *res;


    /* Check the queue is not empty */
    if (queue_empty(queue)) {
        return NULL;
    }

    /* Unlink the first entry from the tail of the queue */
    entry = queue->tail;
    queue->tail = entry->prev;
    res = entry->data;

    if (queue->tail == NULL) {

        /* If doing this has unlinked the last entry in the queue, set
         * head to NULL as well. */
        queue->head = NULL;

    } else {

        /* The new entry at the tail has no next entry. */
        queue->tail->next = NULL;
    }

    /* Free back the queue entry structure */
    queue->free(entry);

    return res;
}

void *
queue_peek_head(queue_t *queue)
{
    if (queue_empty(queue)) {

        return NULL;
    } else {

        return queue->head->data;
    }
}
void *
queue_peek_tail(queue_t *queue)
{
    if (queue_empty(queue)) {

        return NULL;
    } else {

        return queue->tail->data;
    }
}


int_t
queue_empty(queue_t *queue)
{
    return queue->head == NULL;
}
