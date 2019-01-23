#ifndef __THRSAFE_H__
#define __THRSAFE_H__


/*
 * thread-specific data
 */
typedef struct thrsafe_throbj
{
    struct thrsafe_desc *desc;
    size_t thread_num;
    void *perthr_data;
} thrsafe_throbj_t; 



/*
 * descriptor for thread-safety test
 */
typedef struct thrsafe_desc
{
    char *title;          /* test name */
    size_t thread_count;  /* how many threads? */
    size_t loop_count;    /* how many times do test? */

     /* function to initialize test,
      * init_test must initialize all thread-objects
      * so that argument is double-pointer.
      */
    int (*init_test)(thrsafe_throbj_t **);

    /*
     * each thread takes one thread-objects,
     * so that argument is single pointer.
     */
    int (*do_test)(thrsafe_throbj_t *);   /* function to do test */

     /* 
      * verify/final_test take all thread-objects.
      * so that argument is double-pointer.
      */
    int (*verify_test)(thrsafe_throbj_t **); /* function to return result */
    int (*final_test)(thrsafe_throbj_t **); /* function to finalize test */
    
    void *test_data;
} thrsafe_desc_t;


#define THRSAFE_SUCCESS 0
#define THRSAFE_FAIL 1



#endif
