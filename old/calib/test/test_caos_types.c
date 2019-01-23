
#include <calib.h>

#include <caos_types.h>


#define CHECK_BIT_SIZE(t,b)                                          \
    if ((int)sizeof(t) != (int)((b)/8))                              \
    {                                                                \
        printf(#t" is not 8-bit, but %d-bit\n", (int)sizeof(t)*8);   \
        exit(1);                                                     \
    }


int main(void)
{

    CHECK_BIT_SIZE(int8_t,8);
    CHECK_BIT_SIZE(int16_t,16);
    CHECK_BIT_SIZE(int32_t, 32);
    CHECK_BIT_SIZE(int64_t, 64);

    CHECK_BIT_SIZE(INT64_C(1), 64);
    CHECK_BIT_SIZE(INT64_MAX, 64);

    
    return 0;
}
