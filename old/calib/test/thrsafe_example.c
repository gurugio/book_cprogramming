
#include <stdio.h>
#include <stdlib.h>

#include <thrsafe.h>
#include <util.h>

atomic_u64_t example_atomic_count;

int init_example(thrsafe_throbj_t **);
int do_example(thrsafe_throbj_t *);
int final_example(thrsafe_throbj_t **);
int verify_example(thrsafe_throbj_t **);


unsigned long long result_value = 500*5000;
#define TEST_THR_COUNT 1
#define TEST_LOOP_COUNT 50


thrsafe_desc_t global_desc =
{
    "example",
    TEST_THR_COUNT,
    TEST_LOOP_COUNT,
    init_example,
    do_example,
    verify_example,
    final_example,
    (void *)&result_value
};


int init_example(thrsafe_throbj_t **all_throbj)
{
    size_t i;
    
    example_atomic_count = 0;

    for (i = 0; i < global_desc.thread_count; i++)
    {
        (*all_throbj)[i].perthr_data = (void *)&example_atomic_count;
    }

    return THRSAFE_SUCCESS;
}

int do_example(thrsafe_throbj_t *throbj)
{
    //(*(unsigned long long *)(throbj->perthr_data))++;
    atomic_inc(throbj->perthr_data);
    
    return THRSAFE_SUCCESS;
}

int final_example(thrsafe_throbj_t **all_throbj)
{
    all_throbj = all_throbj; /* prevent warning */
    
    return THRSAFE_SUCCESS;
}

int verify_example(thrsafe_throbj_t **all_throbj)
{
    unsigned long long max_count =
        (*all_throbj)[0].desc->thread_count *
        (*all_throbj)[0].desc->loop_count;

    /* non-atomic operation by multi-thread must be broken */
       
    if (max_count == example_atomic_count)
        return THRSAFE_SUCCESS;
    else
        return THRSAFE_FAIL;
}
