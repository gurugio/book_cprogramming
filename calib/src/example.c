/*********************************************************************
 * $Id: 
 *********************************************************************/

#include <stdio.h>
#include <platform.h>

int main(void)
{
    printf("sizeof(int)      =%d\n", sizeof(int));
    printf("sizeof(long)     =%d\n", sizeof(long));
    printf("sizeof(long long)=%d\n", sizeof(long long));
    printf("CPU is <%s>\n", CAOS_CFG_CPU);
    return 0;
}
