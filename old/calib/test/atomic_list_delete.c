

#include <stdio.h>

#include <thrsafe.h>
#include <atomic_list.h>


#define DEBUG
#undef DEBUG

#ifdef DEBUG
#define DPRINTF(fmt, args...) printf("[%s:%d]: " fmt, \
                                     __FUNCTION__, __LINE__, ##args)
#else
#define DPRINTF(fmt, args...)
#endif



#define TEST_DATA_COUNT 10 /* < 26 */
#define TEST_THR_COUNT 50
#define TEST_LOOP_COUNT 100

struct thr_data_st
{
    struct atomic_list *test_list;
    size_t data_count;
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

struct atomic_list global_test_list;
struct thr_data_st global_thr_data[TEST_THR_COUNT];



int init_list_test(thrsafe_throbj_t **all_throbjs)
{
    size_t i,j,k;

    if (atomic_list_init(&global_test_list) < 0)
        return THRSAFE_FAIL;

    DPRINTF("HEAD->%p TAIL->%p\n",
           global_test_list.head,
           global_test_list.tail);

    /* init data for all threads */
    for (j = 0; j < global_desc.thread_count; j++)
    {
        global_thr_data[j].data_count = TEST_DATA_COUNT;
        global_thr_data[j].test_list = &global_test_list;
        
        for (i = 0; i < TEST_DATA_COUNT; i++)
        {
            global_thr_data[j].test_data[i] = (char)('A'+i);
        }
    }

    /* All threads share the same data */
    for (i = 0; i < global_desc.thread_count; i++)
    {
        (*all_throbjs)[i].perthr_data = (void *)&global_thr_data[i];
    }

    /* insert data to be deleted */
    for (k = 0; k < TEST_LOOP_COUNT; k++)
    {
        for (j = 0; j < TEST_DATA_COUNT; j++)
        {
            for (i = 0; i < TEST_THR_COUNT; i++)
            {
                if (atomic_list_insert(&global_test_list,
                                       &(global_thr_data[i].test_data[j])) < 0)
                {
                    return THRSAFE_FAIL;
                }
                //DPRINTF("INSERT<%c>\n", global_thr_data[i].test_data[j]);
            }
        }
    }

    return THRSAFE_SUCCESS;
}

int do_list_test(thrsafe_throbj_t *throbj)
{
    size_t i;
    struct thr_data_st *thrdata =
        (struct thr_data_st *)(throbj->perthr_data);
    
    for (i = 0; i < thrdata->data_count; i++)
    {
        //DPRINTF("DELETE_DATA[%c]\n", (thrdata->test_data)[i]);

        /* every deleting must succeed */
        if (atomic_list_delete(thrdata->test_list,
                               &((thrdata->test_data)[i])) != 0)
        {
            DPRINTF("DELETING FAILED\n");
            return THRSAFE_FAIL;
        }
    }
    
    return THRSAFE_SUCCESS;
}

int final_list_test(thrsafe_throbj_t **all_throbjs)
{
    atomic_list_final(&global_test_list);
    all_throbjs = all_throbjs; /* remove warning */
    return THRSAFE_SUCCESS;
}

int verify_list_test(thrsafe_throbj_t **all_throbjs)
{
    size_t max_count =
        TEST_THR_COUNT*TEST_LOOP_COUNT*TEST_DATA_COUNT;
    size_t count;
    atomic_list_node_t *node = global_test_list.head->next;

    all_throbjs = all_throbjs;

    for (count = 0;
         node != global_test_list.tail;
         count++, node = node->next)
    {
        if (count > max_count) break;
        
        DPRINTF("<%p>\n", node);
        continue;
    }

    DPRINTF("node count=%ld\n", count);
    if (0 == count)
        return THRSAFE_SUCCESS;
    else
        return THRSAFE_FAIL;
}
