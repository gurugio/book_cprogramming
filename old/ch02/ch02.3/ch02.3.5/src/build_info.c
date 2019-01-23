/*********************************************************************
 * $Id: 
 *********************************************************************/

#include <calib.h>

/**
 * return compile bit as char string
 * @return char type string show compiled bit
 */
int32_t build_info_compile_bit(void)
{
    return CALIB_CFG_COMPILE_BIT;
}

/**
 * return build mode as char string
 * @return char type string show build mode
 */
char *build_info_build_mode(void)
{
    return CALIB_CFG_BUILD_MODE;
}



