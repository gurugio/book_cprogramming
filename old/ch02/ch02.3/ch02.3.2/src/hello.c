/*********************************************************************
 * $Id$
 *********************************************************************/

#include <hello.h>

/**
 * return greeting message to test library
 * @return char type string "hello!"
 */
char *say_hello(void)
{
    static char hello[] = "hello!";
    return hello;
}
