

#include <stdio.h>

#include <thrsafe.h>
#include <atomic_list.h>

#define TEST_DATA_COUNT 10 /* < 26 */
#define TEST_THR_COUNT 5
#define TEST_LOOP_COUNT 10


struct list_test_st
{
    struct atomic_list test_list;
    char test_data[TEST_DATA_COUNT];
};

int init_list_test(thrsafe_throbj_t **);
int do_list_test(thrsafe_throbj_t *);
int final_list_test(thrsafe_throbj_t **);
int verify_list_test(thrsafe_throbj_t **);



thrsafe_desc_t global_desc =
{
    "atomic-list test",
    TEST_THR_COUNT,
    TEST_LOOP_COUNT,
    init_list_test,
    do_list_test,
    verify_list_test,
    final_list_test,
    NULL
};

struct list_test_st global_test_insert;


int init_list_test(thrsafe_throbj_t **all_throbjs)
{
    size_t i;

    if (atomic_list_init(&(global_test_insert.test_list)) < 0)
        return THRSAFE_FAIL;

    /*printf("HEAD->%p TAIL->%p\n", global_test_insert.test_list.head,
      global_test_insert.test_list.tail);*/
    
    for (i = 0; i < TEST_DATA_COUNT; i++)
    {
        global_test_insert.test_data[i] = (char)('A'+i);
    }

    /* All threads share the same data */
    for (i = 0; i < global_desc.thread_count; i++)
    {
        (*all_throbjs)[i].perthr_data = (void *)&global_test_insert;
    }

    return THRSAFE_SUCCESS;
}

int do_list_test(thrsafe_throbj_t *throbj)
{
    size_t i;
    struct list_test_st *stest = throbj->perthr_data;
    struct atomic_list *slist = &(stest->test_list);

    for (i = 0; i < TEST_DATA_COUNT; i++)
    {
        //printf("insert<%c>\n", stest->test_data[i]);
        if (atomic_list_insert(slist, &(stest->test_data[i])) < 0)
        {
            return THRSAFE_FAIL;
        }
    }
    
    return THRSAFE_SUCCESS;
}

int final_list_test(thrsafe_throbj_t **all_throbjs)
{
    atomic_list_final(&(global_test_insert.test_list));

    all_throbjs = all_throbjs; /* remove warning */
    return THRSAFE_SUCCESS;
}

int verify_list_test(thrsafe_throbj_t **all_throbjs)
{
    size_t max_count =
        (*all_throbjs)[0].desc->thread_count *                \
        (*all_throbjs)[0].desc->loop_count * TEST_DATA_COUNT;
    size_t count;
    atomic_list_node_t *node = global_test_insert.test_list.head->next;

    /*printf("node->%p next->%p next-data->%c\n", node, node->next,
           *(char *)(node->next->data));
           printf("tail->%p\n", global_test_insert.test_list.tail);*/
    for (count = 0;
         node != global_test_insert.test_list.tail;
         count++, node = node->next)
    {
        //printf("%p\n", node);
        continue;
    }

    /*printf("node count=%d\n", (int)count);*/
    if (max_count == count)
        return THRSAFE_SUCCESS;
    else
        return THRSAFE_FAIL;
}
