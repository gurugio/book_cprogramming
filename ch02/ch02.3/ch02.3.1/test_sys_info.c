/*********************************************************************
 * $Id: 
 *********************************************************************/

#include <calib.h>

int main(void)
{
    printf("== Platform information ==\n");
    printf("Found CPU is <%s>\n", sys_info_cpu_type());
    printf("OS is <%s>\n", sys_info_os_type());
    printf("sizeof(int)       = %d\n", (int32_t)sizeof(int));
    printf("sizeof(long)      = %d\n", (int32_t)sizeof(long));
    printf("sizeof(long long) = %d\n", (int32_t)sizeof(long long));
    return 0;
}
