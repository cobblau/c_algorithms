#ifndef ALI_LIST_H
#define ALI_LIST_H

/**
 * A linux list.
 */

typedef struct list_head list_head_t;

struct list_head {
    struct list_head *next;
    struct list_head *prev;
};

#define LIST_HEAD_INIT(name) {&(name), &(name) }
#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)

static inline void init_list_head(struct list_head *list)
{
    list->next = list;
    list->prev = list;
}

/* add new */
static inline void __list_add(struct list_head *new,
                              struct list_head *prev, struct list_head *next)
{
    next->prev = new;
    new->next  = next;
    new->prev  = prev;
    prev->next = new;
}
static inline void list_add(struct list_head *new, struct list_head *head)
{
    __list_add(new, head, head->next);
}
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
    __list_add(new, head->prev, head);
}

/* delete element */
static inline void list_del(struct list_head *entry)
{
    struct list_head *prev = entry->prev;
    struct list_head *next = entry->next;
    next->prev = prev;
    prev->next = next;

/** These are non-NULL pointers that will result in page faults  
 *  under normal circumstances, used to verify that nobody uses  
 *  non-initialized list entries.  
 */ 
#define LIST_POISON1  ((void*) 0x00100100)
#define LIST_POISON2  ((void*) 0x00200200)
    entry->next = LIST_POISON1;
    entry->prev = LIST_POISON2;
}

/* judge */
static inline int list_empty(struct list_head *head)
{
    return head->next == head;
}

/* list walker */
#define prefetch(x) __builtin_prefetch(x)
#define list_for_each(pos, head)  \
    for(pos = (head)->next;  \
        prefetch(pos->next), pos != (head);  \
        pos = pos->next)
#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next;  \
         pos != (head);  \
         pos = n, n = pos->next)

/* another list walker */
#define list_first_entry(ptr, type, member)  \
    list_entry((ptr)->next, type, member)
#define list_next_entry(pos, member) \
    list_entry((pos)->member.next, typeof(*(pos)), member)
#define list_for_each_entry(pos, head, member) \
    for (pos = list_first_entry(head, typeof(*pos), member); \
         &pos->member != (head); \
         pos = list_next_entry(pos, member))

/* get real struct */
#define list_entry(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))

#endif  /* ALI_LIST_H */
