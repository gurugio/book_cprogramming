/*********************************************************************
 * $Id: 
 *********************************************************************/

#include <calib.h>
#include <sys_info.h>

int main(void)
{
    printf("== Build information ==\n");
    printf("Build mode  = %s\n", build_info_build_mode());
    printf("Compile bit = %d\n", build_info_compile_bit());
    return 0;
}
