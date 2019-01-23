
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include <thrsafe.h>
#include <util.h>

#undef DEBUG

#if defined(DEBUG)
#define MALLOC_HOOK_DEBUG
#define MALLOC_TRACE_DEBUG
#define DPRINTF(fmt, args...) printf("[%s:%d]: " fmt, \
                                     __FUNCTION__, __LINE__, ##args)
#else
#undef MALLOC_HOOK_DEBUG
#undef MALLOC_TRACE_DEBUG
#define DPRINTF(fmt, args...)
#endif


#if defined(MALLOC_TRACE_DEBUG)
#include <mcheck.h>
#endif


#if defined(MALLOC_HOOK_DEBUG)
#include <malloc.h>

atomic_u64_t malloc_lock;
atomic_u64_t free_lock;

atomic_u64_t malloc_counter;
atomic_u64_t free_counter;

/* Prototypes for our hooks.  */
static void my_init_hook(void);
static void *my_malloc_hook(size_t, const void *);
static void my_free_hook(void *, const void *);

/* Variables to save original hooks. */
static void *(*old_malloc_hook)(size_t, const void *);
static void (*old_free_hook)(void *, const void *);

/* Override initializing hook from the C library. */
void (*__malloc_initialize_hook) (void) = my_init_hook;


static void *my_malloc_hook(size_t size, const void *caller)
{
    void *result;

    while (atomic_cas(&malloc_lock, 1, 0) != ATOMIC_SUCCESS)
        continue;
    
    /* Restore all old hooks */
    __malloc_hook = old_malloc_hook;

    /* Call recursively */
    result = malloc(size);

    /* Save underlying hooks */
    old_malloc_hook = __malloc_hook;

    atomic_inc(&malloc_counter);

    caller = caller;
    //DPRINTF("malloc=%d\n", malloc_counter);
    /* DPRINTF() might call malloc(), so protect it too. */
    //DPRINTF("malloc(%u) called from %p returns %p\n",
    //(unsigned int) size, caller, result);

    /* Restore our own hooks */
    __malloc_hook = my_malloc_hook;

    malloc_lock = 1;
    
    return result;
}

static void my_free_hook(void *ptr, const void *caller)
{

    
    while (atomic_cas(&free_lock, 1, 0) != ATOMIC_SUCCESS)
        continue;

    __free_hook = old_free_hook;

    if (ptr == NULL)
    {
        //perror("free NULL");
    }
    else
    {
        free(ptr);
        atomic_inc(&free_counter);
    }

    old_free_hook = __free_hook;

    caller = caller;
    //DPRINTF("free=%d\n", free_counter);
    //DPRINTF("free(%p) from %p\n", ptr, caller);

    __free_hook = my_free_hook;

    free_lock = 1;
}

static void my_init_hook(void)
{
    /* save system default hooks */
    old_malloc_hook = __malloc_hook;
    old_free_hook = __free_hook;

    /* set my hooks */
    __malloc_hook = my_malloc_hook;
    __free_hook = my_free_hook;

    malloc_counter = 0;
    free_counter = 0;
    malloc_lock = 1;
    free_lock = 1;
}
#endif


extern thrsafe_desc_t global_desc;


void *thrsafe_test_body(void *private_data)
{
    thrsafe_throbj_t *thr_data = private_data;
    size_t loop_count;
    
    for (loop_count = 0;
         loop_count < thr_data->desc->loop_count;
         loop_count++)
    {
        if (thr_data->desc->do_test(thr_data) != THRSAFE_SUCCESS)
        {
            break;
        }

        DPRINTF("TID[%3d] loop[%3d]\n",
                (int)thr_data->thread_num,
                (int)thr_data->desc->loop_count);

        sleep(0); /* preempt another thread */
    }

    return NULL;
}

int main(void)
{
    thrsafe_throbj_t *thr_data;
    pthread_t *tid;
    size_t thr_num;

    DPRINTF("THREAD-SAFETY TEST for %s\n", global_desc.title);

#if defined(MALLOC_TRACE_DEBUG)
    setenv("MALLOC_TRACE", "malloc_debug.out", 1);
    mtrace();
#endif

    tid = (pthread_t *)malloc(sizeof(pthread_t) * global_desc.thread_count);

    /* prepare data for threads */
    thr_data = (thrsafe_throbj_t *)malloc(sizeof(thrsafe_throbj_t)
                                          * global_desc.thread_count);

    /* initialize test taken all thread-spedific data */
    if (global_desc.init_test(&thr_data) != THRSAFE_SUCCESS)
    {
        DPRINTF("fail to initialize test\n");
        goto TEST_FAIL;
    }

    DPRINTF("%d-threads will be created\n", (int)global_desc.thread_count);
    
    /* create threads doing test */
    for (thr_num = 0; thr_num < global_desc.thread_count; thr_num++)
    {
        DPRINTF("creating %dth-thread\n", (int)thr_num);
        
        thr_data[thr_num].desc = &global_desc;
        thr_data[thr_num].thread_num = thr_num;
        
        if (pthread_create(&tid[thr_num],
                           NULL,
                           thrsafe_test_body,
                           &thr_data[thr_num]) != 0)
        {
            perror("cannot create threads for test");
            goto THREAD_FAIL;
        }

 
    }

    for (thr_num = 0; thr_num < global_desc.thread_count; thr_num++)
    {
        if (pthread_join(tid[thr_num], NULL) != 0)
        {
            perror("cannot join thread\n");
            goto THREAD_FAIL;
        }
    }

    /* get result of test */
    if (global_desc.verify_test(&thr_data) == THRSAFE_SUCCESS)
    {
        DPRINTF("TEST-SUCCESS\n");
    }
    else
    {
        DPRINTF("TEST-FAIL\n");
    }

    /* finalize test */
    if (global_desc.final_test(&thr_data) != THRSAFE_SUCCESS)
    {
        DPRINTF("cannot finalize test\n");
        goto TEST_FAIL;
    }
    

    free(tid);
    free(thr_data);

#if defined(MALLOC_HOOK_DEBUG)
    DPRINTF("malloc_counter=%llu, free_counter=%llu\n",
           malloc_counter, free_counter);
#endif
    
    return 0;

TEST_FAIL:
    goto FINAL_FAIL;
    
THREAD_FAIL:
    goto FINAL_FAIL;

FINAL_FAIL:
    free(thr_data);
    free(tid);

    return 0;
}
