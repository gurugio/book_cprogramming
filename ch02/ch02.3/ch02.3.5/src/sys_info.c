/*********************************************************************
 * $Id$
 *********************************************************************/

#include <sys_info.h>


/**
 * return CPU information as char string
 * @return char type string show CPU type
 */
char *sys_info_cpu_type(void)
{
    return CALIB_CFG_CPU;
}

/**
 * return OS information as char string
 * @return char type string show OS type
 */
char *sys_info_os_type(void)
{
    return CALIB_CFG_OS;
}


