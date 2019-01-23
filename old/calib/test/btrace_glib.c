#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include <btrace.h>

#define EXIT_ERR(ret_val) do {\
        if (ret_val < 0) { printf("EXIT by return value %d at %d\n", ret_val, __LINE__); exit(ret_val); } \
    } while (0);

btrace_t *test_bt;

void c(void)
{
    /* Send SIGINT to myself */
    kill(getpid(), SIGINT);
}


void b(void)
{
    c();
}

void a(void)
{
    b();
}


void test_console(void)
{
    int ret;
    
    btrace_get_instance(&test_bt, BTRACE_TYPE_GLIB);

    if (test_bt == NULL)
    {
        perror("fail to create back-tracer");
        exit(1);
    }
    
    ret = test_bt->init(test_bt, BTRACE_ATTR_CONSOLE);
    EXIT_ERR(ret);

    ret = test_bt->trace(test_bt);
    EXIT_ERR(ret);
    
    ret = test_bt->final(test_bt);
    EXIT_ERR(ret);
    
    btrace_free_instance(test_bt);
}

void test_file(void)
{
    int ret;
    struct stat st;

    /* remove log file */
    remove(btrace_file_name);
    
    btrace_get_instance(&test_bt, BTRACE_TYPE_GLIB);

    if (test_bt == NULL)
    {
        perror("fail to create back-tracer");
        exit(1);
    }

    ret = test_bt->init(test_bt, BTRACE_ATTR_FILE);
    EXIT_ERR(ret);

    ret = test_bt->trace(test_bt);
    EXIT_ERR(ret);
    
    ret = test_bt->final(test_bt);
    EXIT_ERR(ret);
    
    btrace_free_instance(test_bt);
    EXIT_ERR(ret);
    
    if (stat(btrace_file_name, &st) < 0)
    {
        perror("fail to create btrace file");
        exit(1);
    }
    
    if (st.st_size <= 0)
    {
        printf("btrace file is created but no data written\n");
        exit(1);
    }
}

void test_signal(void)
{
    int ret;

    /* remove log file */
    remove(btrace_file_name);
    
    btrace_get_instance(&test_bt, BTRACE_TYPE_GLIB);

    if (test_bt == NULL)
    {
        perror("fail to create back-tracer");
        exit(1);
    }
    
    ret = test_bt->init(test_bt, BTRACE_ATTR_SIGNAL | BTRACE_ATTR_FILE);
    EXIT_ERR(ret);

    /* start calling-chains */
    a();

}


int main(void)
{
    pid_t child;
    
    /* test_console(); test_file() is the same */
    test_file();

    if ((child = fork()) == 0)
    {
        /* children test back-tracing when signal */
        test_signal();
    }
    else
    {
        /* children is terminated by signal and generate log file,
         * check log file is generated correctly */
        struct stat st;

        waitpid(child, NULL, 0);
        
        if (stat(btrace_file_name, &st) < 0)
        {
            perror("fail to create btrace file");
            exit(1);
        }

        if (st.st_size <= 0)
        {
            printf("btrace file is created but no data written\n");
        }
    }

    return 0;
}

