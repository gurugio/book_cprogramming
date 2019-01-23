#ifndef __BTRACE_H__
#define __BTRACE_H__

typedef enum btrace_type_t
{
    BTRACE_TYPE_GLIB = 0, /* using GLIB functions */
    BTRACE_TYPE_BUILTIN, /* using GCC built-in functions */
    BTRACE_TYPE_SIZE
} btrace_type_t;


#define BTRACE_BIT_CONSOLE 0 /* print callstack on console */
#define BTRACE_BIT_FILE   1        /* write callstack in file */
#define BTRACE_BIT_SIGNAL 2      /* print callstack by signal */

typedef enum btrace_attr_t
{
    BTRACE_ATTR_NOOPT = 0,
    BTRACE_ATTR_CONSOLE = (1 << BTRACE_BIT_CONSOLE), /* print callstack on console */
    BTRACE_ATTR_FILE = (1 << BTRACE_BIT_FILE),        /* write callstack in file */
    BTRACE_ATTR_SIGNAL = (1 << BTRACE_BIT_SIGNAL)      /* print callstack by signal */
} btrace_attr_t;


typedef struct btrace_t
{
    int (*init)(struct btrace_t *btracer, btrace_attr_t attr);
    int (*trace)(struct btrace_t *btracer);
    int (*final)(struct btrace_t *btracer);
    int attr;
    
    void **private_data;
    size_t addr_count;
    FILE *stream;
} btrace_t;


#define BTRACE_DEFAULT_DEPTH 16

extern const char *btrace_file_name;


int btrace_glib_init(btrace_t *btracer, btrace_attr_t attr);
int btrace_glib_trace(btrace_t *btracer);
int btrace_glib_final(btrace_t *btracer);

int btrace_builtin_init(btrace_t *btracer, btrace_attr_t attr);
int btrace_builtin_trace(btrace_t *btracer);
int btrace_builtin_final(btrace_t *btracer);

int btrace_get_instance(btrace_t **ptr_btracer, btrace_type_t type);
int btrace_free_instance(btrace_t *ptr_btracer);



#endif
