
#include <calib.h>

#include <util.h>
#include <atomic_list.h>


#ifdef DEBUG
#define DPRINTF(fmt, args...) printf("[%s:%d]: " fmt, \
                                     __FUNCTION__, __LINE__, ##args)
#else
#define DPRINTF(fmt, args...)
#endif



inline void atomic_list_alloc_node(atomic_list_node_t **node)
{
    *node = (atomic_list_node_t *)malloc(
        sizeof(atomic_list_node_t));
}

inline void atomic_list_free_node(atomic_list_node_t *node)
{
    free((void *)node);
    node = NULL;
}

inline int atomic_list_init(struct atomic_list *alist)
{
    atomic_list_alloc_node(&(alist->head));
    if (alist->head == NULL)
        return -1;
    
    atomic_list_alloc_node(&(alist->tail));
    if (alist->tail == NULL)
    {
        free((void *)(alist->head));
        return -1;
    }
    
    alist->head->data = NULL;
    alist->tail->data = NULL;
    alist->head->next = alist->tail;
    //alist->tail->next = alist->head; /* circular */
    alist->tail->next = NULL;

    return 0;
}

inline void atomic_list_final(struct atomic_list *alist)
{
    atomic_list_node_t *old_cur;
    atomic_list_node_t *cur;

    if (alist == NULL)
        return;

    if (alist->head == NULL || alist->tail == NULL)
        return;
    
    /* clear list */
    cur = alist->head->next;
    
    while (cur != alist->tail)
    {
        old_cur = cur;
        cur = cur->next;
        atomic_list_free_node(old_cur);
    }

    atomic_list_free_node(alist->head);
    atomic_list_free_node(alist->tail);

    alist->head = NULL;
    alist->tail = NULL;
}

int atomic_list_insert(struct atomic_list *to_list,
                       void *data)
{
    volatile atomic_list_node_t *new_node;
    volatile atomic_list_node_t *right, *left;

    atomic_list_alloc_node(&new_node);
    if (new_node == NULL)
        return -1;

    do
    {
        /* insert next of head */
        left = to_list->head;
        right = to_list->head->next;

        new_node->next = right;
        new_node->data = data;
    } while (atomic_cas(&(left->next), (u64)right, (u64)new_node) ==
             ATOMIC_FAIL);

    return 0;
}


atomic_list_node_t *atomic_list_set_mark(atomic_list_node_t *node)
{
    /* set 0-bit */
    return (atomic_list_node_t *)((u64)node + 0x1);
}

atomic_list_node_t *atomic_list_del_mark(atomic_list_node_t *node)
{
    /* clear 0-bit */
    return (atomic_list_node_t *)((u64)node & (u64)(~0x1));
}

int atomic_list_check_mark(atomic_list_node_t *node)
{
    u64 addr = (u64)node;
    if (addr & 0x1)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* find a node and return itself and left of it */
atomic_list_node_t *atomic_list_search(struct atomic_list *alist,
                                            void *key,
                                            atomic_list_node_t **left)
{
    atomic_list_node_t *cur_left;
    atomic_list_node_t *cur;

TRY_AGAIN:
    cur_left = alist->head;
    cur = alist->head->next;
    
    while (cur != alist->tail)
    {
        if (cur->data == key)
        {
            if (left != NULL)
            {
                *left = cur_left;
            }
            break;
        }

        cur_left = cur;
        cur = atomic_list_del_mark(cur->next);
    }

    if (cur == alist->tail) /* non-exist */
        return NULL;

    /*
     * It is successful only When current node and
     * left node aare adjacent and both are not marked,
     */

    /* Other thread try to remove left or current node,
     * we have to try again. */
    if (atomic_list_check_mark((*left)->next) == TRUE ||
        atomic_list_check_mark(cur->next) == TRUE)
    {
        DPRINTF("3\n");
        goto TRY_AGAIN;
    }
    else if (atomic_list_del_mark((*left)->next) != cur) /* not adjacent */
    {
        DPRINTF("2\n");
        goto TRY_AGAIN;
    }
    else
    {
        return cur;
    }
}

int atomic_list_delete(struct atomic_list *from_list,
                       void *data)
{
    atomic_list_node_t *del_node, *left;
    atomic_list_node_t *del_node_next;
    int ret;

TRY_AGAIN:
    do
    {
        del_node = atomic_list_search(from_list, data, &left);

        if (del_node == NULL)
        {
            ret = -1;
            goto SEARCH_FAIL;
        }

        del_node_next = del_node->next;

        if (atomic_list_check_mark(del_node_next) == FALSE)
        {
            /* 1st CAS : set mark */
            if (atomic_cas(&(del_node->next),
                           (u64)del_node_next,
                           (u64)atomic_list_set_mark(del_node_next))
                == ATOMIC_SUCCESS)
            {
                /* go to 2nd CAS */
                break;
            }
            else
            {
                /* failure of CAS, try again */
                continue;
            }
        }
        else
        {
            /* another thread set mark, search removal node again,
             * If it is removed by other thread, search will fail and
             * delete will fail. It is OK and not error,
             * because removing is successed.
             */
            continue;
        }
    } while (TRUE);


    /* 2nd CAS */
    if (atomic_cas(&(left->next), (u64)del_node, (u64)del_node_next)
        != ATOMIC_SUCCESS)
    {
        /* Left node is marked by another thread
         * so that left node cannot be modified.
         * Therefore we cannot delete del_node.
         * Remove mark and try again.
         */
        DPRINTF("1 ");
        
        if (atomic_cas(&(del_node->next),
                       (u64)atomic_list_set_mark(del_node_next),
                       (u64)atomic_list_del_mark(del_node_next))
            == ATOMIC_SUCCESS)
        {
            DPRINTF("X\n");
        }
        
        goto TRY_AGAIN;
    }

    /* free list-node and return only data */
    atomic_list_free_node(del_node);
    return 0;

SEARCH_FAIL:
    return ret;
}


int atomic_list_delete_first(struct atomic_list *from_list,
                             void **data)
{
    atomic_list_node_t *del_node, *left;
    atomic_list_node_t *del_node_next;
    int ret;

TRY_AGAIN:
    do
    {
        del_node = from_list->head->next;
        left = from_list->head;
        
        if (del_node == NULL)
        {
            ret = -1;
            goto SEARCH_FAIL;
        }

        del_node_next = del_node->next;

        if (atomic_list_check_mark(del_node_next) == FALSE)
        {
            /* 1st CAS : set mark */
            if (atomic_cas(&(del_node->next),
                           (u64)del_node_next,
                           (u64)atomic_list_set_mark(del_node_next))
                == ATOMIC_SUCCESS)
            {
                /* go to 2nd CAS */
                break;
            }
            else
            {
                /* failure of CAS, try again */
                continue;
            }
        }
        else
        {
            /* another thread set mark, search removal node again,
             * If it is removed by other thread,
             * read the first node and try to delete again.
             */
            continue;
        }
    } while (TRUE);


    /* 2nd CAS */
    if (atomic_cas(&(left->next), (u64)del_node, (u64)del_node_next)
        != ATOMIC_SUCCESS)
    {
        /* Left node is marked by another thread
         * so that left node cannot be modified.
         * Therefore we cannot delete del_node.
         * Remove mark and try again.
         */
        DPRINTF("1 ");
        
        if (atomic_cas(&(del_node->next),
                       (u64)atomic_list_set_mark(del_node_next),
                       (u64)atomic_list_del_mark(del_node_next))
            == ATOMIC_SUCCESS)
        {
            DPRINTF("X\n");
        }
        
        goto TRY_AGAIN;
    }

    /* free list-node and return only data */
    *data = del_node->data;
    atomic_list_free_node(del_node);
    return 0;
    
SEARCH_FAIL:
    return ret;
}
