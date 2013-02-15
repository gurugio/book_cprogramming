

#include "serial.h"


int verify_product(char *input, char *target, int digit)
{
    printf("Verification Product..\n");
    
    return COMPARE_STRING_DATA(input, target, digit);
}

int verify_expire(char *input, char *target, int digit)
{
    printf("Verification Expire Data..\n");
    return COMPARE_STRING_DATA(input, target, digit);
}

int verify_id(char *input, char *target, int digit)
{
    printf("Verification ID..\n");
    return COMPARE_STRING_DATA(input, target, digit);
}


serial_data serial_data_table[] =
{
    {
        "PRODUCT ID",
        2,
        "This is product-id",
        verify_product
    },
    {
        "EXPIRE DATE",
        6,
        "This is expire date",
        verify_expire
    },
    {
        "USER ID",
        4,
        "This is user-id",
        verify_id
    },
    SERIAL_SENTINEL 
};


serial_desc my_serial_desc =
{
    "serial for greetea",
    serial_data_table
};


/**
 * g_serial_data must be declared with user's serial-data
 */
serial_desc *g_serial_data = &my_serial_desc;


