

#include <stdio.h>

#include <atomic_list.h>

#define MTRACE_DEBUG 1
#undef MTRACE_DEBUG

#if defined(DEBUG)
#define DPRINTF(fmt, args...) printf("[%s:%d]: " fmt,                   \
                                     __FUNCTION__, __LINE__, ##args)
#else
#define DPRINTF(fmt, args...)
#endif

#if defined(MTRACE_DEBUG)
#include <mcheck.h>
#endif

int main(void)
{
    struct atomic_list test_list;
    char test_data[] = {'a','b','c','d','e','f', (char)0};
    char result_data[10];
    int i, j, ret;
    atomic_list_node_t *cur;

#if defined(MTRACE_DEBUG)
    setenv("MALLOC_TRACE", "malloc_debug.out", 1);
    mtrace();
#endif
    
    if (atomic_list_init(&test_list) < 0)
    {
        ret = -1;
        goto UNITTEST_FAIL;
    }

    /*
     * test insert
     */
    
    i = 0;
    while (test_data[i] != 0)
    {
        if (atomic_list_insert(&test_list, (void *)&test_data[i]) < 0)
        {
            ret = -1;
            goto UNITTEST_FAIL;
        }

        i++;
    }


    cur = test_list.head->next;

    i = 0;
    while (cur != test_list.tail)
    {
        DPRINTF("%p-->%p\n", cur, atomic_list_set_mark(cur));

        result_data[i] = *(char *)(cur->data);
        cur = cur->next;
        i++;
    }

    for (j = 0; i > 0; i--, j++)
    {
        DPRINTF("result=%c, test=%c\n",
               result_data[j], test_data[i-1]);
        if (result_data[j] != test_data[i-1])
        {
            ret = -2;
            goto UNITTEST_FAIL;
        }

    }

    for (i = 0; test_data[i] != 0; i++)
    {
        atomic_list_node_t *left, *found;

        found = atomic_list_search(&test_list, &test_data[i], &left);

        if (found == NULL)
        {
            ret = -3;
            goto UNITTEST_FAIL;
        }
    }

    {
        char wrong_data = (char)0xff;
        atomic_list_node_t *found;
        
        found = atomic_list_search(&test_list, &wrong_data, NULL);
        if (found != NULL)
        {
            ret = -4;
            goto UNITTEST_FAIL;
        }
    }

    {
        i = 0;
        while (test_data[i] != 0)
        {
            if (atomic_list_delete(&test_list, (void *)&test_data[i]) < 0)
            {
                ret = -1;
                goto UNITTEST_FAIL;
            }

            DPRINTF("DEL<%c>\n", test_data[i]);
            i++;
        }
    }

    atomic_list_final(&test_list);

    return 0;
    
UNITTEST_FAIL:
    atomic_list_final(&test_list);
    return ret;
}

