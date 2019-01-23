
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>


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
#define TEST_LOOP_COUNT 200


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


struct thr_data_st
{
    struct atomic_list *test_list;
    int fd;
} global_thrdata[TEST_THR_COUNT];

struct atomic_list global_test_list;

int init_list_test(thrsafe_throbj_t **all_throbjs)
{
    size_t i;
    int random_fd;

    if (atomic_list_init(&global_test_list) < 0)
        return THRSAFE_FAIL;

    /* do not use thread-specific data */
    
    /*printf("HEAD->%p TAIL->%p\n", global_test_insert.test_list.head,
      global_test_insert.test_list.tail);*/
    
    /* /dev/urandom generate numbers fast!, do not use /dev/random. */
    random_fd = open("/dev/urandom", O_RDONLY);
    if (random_fd < 0)
    {
        perror("open /dev/random failed");
        return THRSAFE_FAIL;
    }

    for (i = 0; i < TEST_THR_COUNT; i++)
    {
        global_thrdata[i].test_list = &global_test_list;
        global_thrdata[i].fd = random_fd;
        (*all_throbjs)[i].perthr_data = (void *)&global_thrdata[i];
    }
    
    return THRSAFE_SUCCESS;
}

int do_list_test(thrsafe_throbj_t *throbj)
{
    char random_data[16];
    unsigned char wait_time;

    struct thr_data_st *thrdata = throbj->perthr_data;
    int *insert_data;


    insert_data = malloc(sizeof(int));
    if (insert_data == NULL)
        return THRSAFE_FAIL;
    
    (*insert_data) = throbj->thread_num;
    
    /*
     * test thread works:
     * 1. insert random-generated character
     * 2. delay
     * 3. delete the character
     */

    if (atomic_list_insert(thrdata->test_list, insert_data) < 0)
    {
        DPRINTF("insert failed\n");
        goto RET_FAIL;
    }

    if (read(thrdata->fd, random_data, 16) < 0)
    {
        perror("read random failed");
        goto RET_FAIL;
    }

    wait_time = random_data[0];
    DPRINTF("wait<%d>\n", wait_time);

    usleep(wait_time * 100);

    if (atomic_list_delete(thrdata->test_list, insert_data) < 0)
    {
        DPRINTF("delete failed\n");
        goto RET_FAIL;
    }

    free(insert_data);
    
    return THRSAFE_SUCCESS;

RET_FAIL:

    free(insert_data);

    return THRSAFE_FAIL;
}

int final_list_test(thrsafe_throbj_t **all_throbjs)
{
    atomic_list_final(&global_test_list);

    all_throbjs = all_throbjs; /* remove warning */

    return THRSAFE_SUCCESS;
}

int verify_list_test(thrsafe_throbj_t **all_throbjs)
{
    all_throbjs = all_throbjs;

#if defined(DEBUG)
    atomic_list_node_t *node = global_test_list.head->next;
       
    for (; node != global_test_list.tail; node = node->next)
    {
        printf("node<%d>\n", *(int *)node->data);
    }
#endif

    if (global_test_list.head->next == global_test_list.tail)
        return THRSAFE_SUCCESS;
    else
        return THRSAFE_FAIL;
}
