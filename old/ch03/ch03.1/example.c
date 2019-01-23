

#include <stdio.h>

/*$$start handlers */

void handle_error0(void)
{
    printf("Handle Error #0\n");
}
void handle_error1(void)
{
    printf("Handle Error #1\n");
}
void handle_error2(void)
{
    printf("Handle Error #2\n");
}
void handle_error3(void)
{
    printf("Handle Error #3\n");
}
/*$$end handlers */

/*$$start badcase */
void bad_error_process(int err)
{
    if (err == 0)
    {
        handle_error0();
    }
    else if (err == 1)
    {
        handle_error1();
    }
    else if (err == 2)
    {
        handle_error2();
    }
    else if (err == 3)
    {
        handle_error3();
    }
    else
    {
        printf("Unidentified error\n");
    }
}
/*$$end basecase */

/*$$start goodcase */
void good_error_process(int err)
{
    typedef struct err_table
    {
        int err_num;
        void (*err_handler)(void);
    } err_table;

    int i;

    err_table table[] =
    {
        {0, handle_error0},
        {1, handle_error1},
        {2, handle_error2},
        {3, handle_error3}
    };

    for (i = 0; i < sizeof(table)/sizeof(err_table); i++)
    {
        if (err == table[i].err_num)
            table[i].err_handler();
    }

}
/*$$end goodcase */


/*$$start anothercase */
#define MAX_ERROR_NUM 3
typedef struct err_table
{
    int err_num;
    void (*err_handler)(void);
} err_table;
err_table global_err_table[MAX_ERROR_NUM+1];

#define SETUP_ERR_TABLE(num,handler) \
    global_err_table[num].err_num = num;                             \
    global_err_table[num].err_handler = handler;
#define CALL_ERR_HANDLER(num) global_err_table[num].err_handler()


void another_error_process(int err)
{
    /* separate table setup and table reference */
    SETUP_ERR_TABLE(0, handle_error0);
    SETUP_ERR_TABLE(1, handle_error1);
    SETUP_ERR_TABLE(2, handle_error2);
    SETUP_ERR_TABLE(3, handle_error3);

    CALL_ERR_HANDLER(err);
}
/*$$end anothercase */


int main(void)
{

    bad_error_process(3);
    good_error_process(3);
    another_error_process(3);

    return 0;
}
