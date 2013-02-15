
#include <calib.h>

#include <btrace.h>



const char *btrace_file_name = "btrace.txt";
const char *btrace_file_mode = "w";

btrace_t *global_btracer;

static int btrace_set_signal(void (*handler)(int, siginfo_t *, void *));

static void btrace_signal_handler(int signum, siginfo_t *siginfo, void *ucontext)
{
    ucontext = ucontext;

    if ((global_btracer->attr & BTRACE_ATTR_FILE) != 0)
    {
        fprintf(global_btracer->stream, "signal[%d] occurs at %p\n",
                signum, siginfo->si_addr);
    }
    else if ((global_btracer->attr & BTRACE_ATTR_CONSOLE) != 0)
    {
        fprintf(stdout, "signal[%d] occurs at %p\n",
                signum, siginfo->si_addr);
    }
    else
    {
        /* nothing */
    }
    
    global_btracer->trace(global_btracer);

    return;
}


static int btrace_set_signal(void (*handler)(int, siginfo_t *, void *))
{
    static int sig_nums[] =
        {
            SIGINT,
            SIGILL,
            SIGABRT,
            SIGFPE,
            SIGSEGV,
            SIGTRAP,
            SIGBUS,
            0
        };

    struct sigaction sigact;
    int i;

    if (sigemptyset(&sigact.sa_mask) < 0)
    {
        perror("get empty-signal error");
        return -1;
    }

    if (handler != NULL)
    {
        sigact.sa_sigaction = btrace_signal_handler;
        sigact.sa_flags     = SA_RESETHAND | SA_SIGINFO;
    }
    else
    {
        sigact.sa_handler = SIG_DFL;
        sigact.sa_flags   = SA_RESETHAND;
    }

    for (i = 0; sig_nums[i] != 0; i++)
    {
        if (sigaction(sig_nums[i], &sigact, NULL) < 0)
        {
            perror("set signal action error");
            return -1;
        }
    }

    return 0;
}



int btrace_glib_init(btrace_t *btracer, btrace_attr_t attr)
{
    if (btracer == NULL)
        return -1;
    
    if (attr == BTRACE_ATTR_NOOPT)
    {
        printf("attribute setting error\n");
        return -1;
    }
    else if ((attr & BTRACE_ATTR_FILE) == 0 &&
             (attr & BTRACE_ATTR_CONSOLE) == 0)
    {
        printf("One of BTRACE_ATTR_FILE or BTRACE_ATTR_CONSOLE must be set\n");
        return -2;
    }
    else
    {
        btracer->attr = attr;
    }
    
    /* set file stream*/
    if ((attr & BTRACE_ATTR_FILE) != 0)
    {
        btracer->stream = fopen(btrace_file_name, btrace_file_mode);
        if (btracer->stream == NULL)
        {
            perror("file open error");
            return -3;
        }
    }
    else
    {
        btracer->stream = NULL;
    }
    
    /* set signal handling */
    if ((attr & BTRACE_ATTR_SIGNAL) != 0)
    {
        if (btrace_set_signal(btrace_signal_handler) != 0)
            return -3;

        global_btracer = btracer;
    }
    else
    {
        global_btracer = NULL;
    }

    btracer->private_data = (void **)calloc(sizeof(void *), BTRACE_DEFAULT_DEPTH);
    btracer->addr_count = BTRACE_DEFAULT_DEPTH;
    
    return 0;
}

int btrace_glib_final(btrace_t *btracer)
{
    btracer->attr = BTRACE_ATTR_NOOPT;
    
    /* close file */
    if (btracer->stream != NULL)
        fclose(btracer->stream);

    /* destroy signal handler */
    if (global_btracer != NULL)
        btrace_set_signal(NULL);

    free(btracer->private_data);
    btracer->private_data = 0;
    
    btracer->addr_count = 0;
    
    return 0;
}

int btrace_glib_trace(btrace_t *btracer)
{
    char **frame_strings;
    size_t backtrace_size;
    int i;


    /* make enough buffer to store tracing addresses */
    do
    {
        backtrace_size = backtrace(btracer->private_data, btracer->addr_count);
        frame_strings = backtrace_symbols(btracer->private_data, backtrace_size);

        if (backtrace_size < btracer->addr_count)
        {
            break;
        }
        else
        {
            btracer->addr_count = btracer->addr_count * 2;
            btracer->private_data = realloc(btracer->private_data,
                                            btracer->addr_count * sizeof(void *));
        }
    } while (1);

    for (i = 0; i < (int)backtrace_size; i++)
    {
        if ((btracer->attr & BTRACE_ATTR_FILE) != 0)
        {
            fprintf(btracer->stream,
                    "%d: [%p] %s\n", i, btracer->private_data[i], frame_strings[i]);
        }
        else if ((btracer->attr & BTRACE_ATTR_CONSOLE) != 0)
        {
            fprintf(stdout,
                    "%d: [%p] %s\n", i, btracer->private_data[i], frame_strings[i]);
        }
    }

    /* If the backtracer works when signal,
     * the program can be terminated before message are printed,
     * so flushing stream is need */
    fflush(btracer->stream);

    /* after printing message completes */
    free(frame_strings);
    return 0;
}



typedef struct frame_t
{
    struct frame_t *ebp; /* Next frame address */
    void *ret; /* caller address */
} frame_t;


int btrace_builtin_init(btrace_t *btracer, btrace_attr_t attr)
{
    if (btracer == NULL)
        return -1;
    
    if (attr == BTRACE_ATTR_NOOPT)
    {
        printf("attribute setting error\n");
        return -1;
    }
    else if ((attr & BTRACE_ATTR_FILE) == 0 &&
             (attr & BTRACE_ATTR_CONSOLE) == 0)
    {
        printf("One of BTRACE_ATTR_FILE or BTRACE_ATTR_CONSOLE must be set\n");
        return -2;
    }
    else
    {
        btracer->attr = attr;
    }
    
    /* set file stream */
    if ((attr & BTRACE_ATTR_FILE) != 0)
    {
        btracer->stream = fopen(btrace_file_name, btrace_file_mode);
        if (btracer->stream == NULL)
        {
            perror("file open error");
            return -3;
        }
    }
    else
    {
        btracer->stream = NULL;
    }
    
    /* set signal handling */
    if ((attr & BTRACE_ATTR_SIGNAL) != 0)
    {
        if (btrace_set_signal(btrace_signal_handler) != 0)
            return -3;

        global_btracer = btracer;
    }
    else
    {
        global_btracer = NULL;
    }

    btracer->private_data = (void **)calloc(sizeof(void *), BTRACE_DEFAULT_DEPTH);
    btracer->addr_count = BTRACE_DEFAULT_DEPTH;
    
    return 0;
}


int btrace_builtin_final(btrace_t *btracer)
{
    btracer->attr = BTRACE_ATTR_NOOPT;
    
    /* close file */
    if (btracer->stream != NULL)
        fclose(btracer->stream);

    /* destroy signal handler */
    if (global_btracer != NULL)
        btrace_set_signal(NULL);

    free(btracer->private_data);
    btracer->private_data = NULL;
    btracer->addr_count = 0;
    
    return 0;
}

int btrace_builtin_trace(btrace_t *btracer)
{
    frame_t *frame = __builtin_frame_address(0);

    while (frame != NULL)
    {
        if ((btracer->attr & BTRACE_ATTR_FILE) != 0)
            fprintf(btracer->stream, "%p\n", frame->ret);

        if ((btracer->attr & BTRACE_ATTR_CONSOLE) != 0)
            fprintf(stdout, "%p\n", frame->ret);
        
        frame = frame->ebp;
    }

    return 0;
}



btrace_t btracer_table[BTRACE_TYPE_SIZE] =
{
    {
        btrace_glib_init,
        btrace_glib_trace,
        btrace_glib_final,
        BTRACE_ATTR_NOOPT,
        NULL,
        0,
        NULL
    },
    {
        btrace_builtin_init,
        btrace_builtin_trace,
        btrace_builtin_final,
        BTRACE_ATTR_NOOPT,
        NULL,
        0,
        NULL
    }
};


/* allocate memory for btrace_t structure,
 * and set functions.
 * These can be done by user directly.
 * But because the btrace_t structure can be changed,
 * so that using the get_btracer() is safe.
 */
int btrace_get_instance(btrace_t **ptr_btracer, btrace_type_t type)
{
    btrace_t *new_btracer;

    new_btracer = malloc(sizeof(btrace_t));
    if (new_btracer == NULL)
    {
        perror("create btracer error");
        *ptr_btracer = NULL;
        return -1;
    }

    if (type == BTRACE_TYPE_BUILTIN)
    {
        new_btracer->init = btrace_builtin_init;
        new_btracer->trace = btrace_builtin_trace;
        new_btracer->final = btrace_builtin_final;
    }
    else
    {
        new_btracer->init = btrace_glib_init;
        new_btracer->trace = btrace_glib_trace;
        new_btracer->final = btrace_glib_final;
    }

    new_btracer->attr = BTRACE_ATTR_NOOPT;
    *ptr_btracer = new_btracer;
    
    return 0;
}

int btrace_free_instance(btrace_t *ptr_btracer)
{
    free(ptr_btracer);
    
    return 0;
}
