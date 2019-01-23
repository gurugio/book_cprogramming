#ifndef __ATOMIC_LIST_H__
#define __ATOMIC_LIST_H__

#include <calib.h>

typedef volatile struct atomic_list_node
{
    void *data;
    volatile struct atomic_list_node *next;
} atomic_list_node_t;

struct atomic_list
{
    atomic_list_node_t *head;
    atomic_list_node_t *tail;
};


inline void atomic_list_alloc_node(atomic_list_node_t **node);
inline void atomic_list_free_node(atomic_list_node_t *node);
inline int atomic_list_init(struct atomic_list *alist);
inline void atomic_list_final(struct atomic_list *alist);
atomic_list_node_t *atomic_list_set_mark(atomic_list_node_t *node);
atomic_list_node_t *atomic_list_del_mark(atomic_list_node_t *node);
    
int atomic_list_insert(struct atomic_list *to_list,
                       void *data);
atomic_list_node_t *atomic_list_search(struct atomic_list *alist,
                                       void *key,
                                       atomic_list_node_t **left);
int atomic_list_delete(struct atomic_list *from_list,
                       void *data);

int atomic_list_delete_first(struct atomic_list *from_list,
                       void **data);

int unittest_atomic_list(void);


#endif
