#include <stdint.h>
#include <string.h>

#include "serial.h"

#if defined(VERBOSE_MODE)
#define PRINT(fmt, args...) printf("[%s:%d]: " fmt, \
                                     __FUNCTION__, __LINE__, ##args)
#else
#define PRINT(fmt, args...)
#endif


int serial_init(serial_desc *desc)
{
    serial_data *data_array = desc->serial_data_array;
    int i;
    int ret = 0;
    
    PRINT("initialize serial generator\n");
    PRINT("making serial for %s\n", desc->product_name);
    PRINT("verification data are:\n");

    for (i = 0; ; i++)
    {
        if (data_array[i].name == NULL && data_array[i].digit == 0)
        {
            ret = 1;
            break;
        }
        else if (data_array[i].name != NULL && data_array[i].digit == 0)
        {
            ret = 2;
            break;
        }
        else if (data_array[i].name == NULL && data_array[i].digit != 0)
        {
            ret = 3;
            break;
        }
        else if (data_array[i].verify_func == NULL)
        {
            ret = 4;
            break;
        }
        else
        {
            PRINT("[%s(%d)]\n", data_array[i].name, data_array[i].digit);
        }
    }

    if (ret != 0)
    {
        printf("incorrect information at %d-th descriptor\n", ret);
    }
    
    return ret;
}

void serial_final(serial_desc *desc)
{
    PRINT("finish serial generator\n");

    return;
}

char *my_encode(char *data, size_t len)
{
    for (;len > 0; len--)
    {
        *data = (*data) + 1;
        data++;
    }
    return data;
}

char *my_decode(char *data, size_t len)
{
   for (;len > 0; len--)
   {
       *data = (*data) - 1;
       data++;
   }
   return data;
}

void serial_generate(serial_desc *desc)
{
    int i;
    serial_data *data_array = desc->serial_data_array;
    char buf[SERIAL_LENGTH_MAX];
    char result[SERIAL_LENGTH_MAX];

    result[0] = '\0';
    
    for (i = 0; data_array[i].name != NULL; i++)
    {
        memset(buf, 0, SERIAL_LENGTH_MAX);
        printf("insert [%s(%d)]: ", data_array[i].name, data_array[i].digit);
        scanf("%s", buf);

        my_encode(buf, data_array[i].digit);
        strcat(result, buf);
    }

    printf("%s\n", result);

    return;
}

void serial_verify(serial_desc *desc)
{
    int i;
    serial_data *data_array = desc->serial_data_array;
    char serial[SERIAL_LENGTH_MAX];
    int serial_index;
    char buf[SERIAL_LENGTH_MAX];
    int ret = 0;

    memset(serial,0,SERIAL_LENGTH_MAX);
    printf("insert serial: ");
    scanf("%s", serial);

    serial_index = 0;
    for (i = 0; data_array[i].name != NULL; i++)
    {
        printf("insert [%s(%d)]: ", data_array[i].name, data_array[i].digit);
        scanf("%s", buf);

        if (data_array[i].digit != strnlen(buf, SERIAL_LENGTH_MAX))
        {
            printf("Data digit error: input %d-digit\n", data_array[i].digit);
        }

        // encode user data amd compare it with serial
        my_encode(buf, strlen(buf));

        ret = data_array[i].verify_func(buf,
                                        &serial[serial_index],
                                        data_array[i].digit);
        
        if (ret == SERIAL_VERIFY_FAIL)
        {
            break;
        }
        else
        {
            PRINT("OK\n");
        }

        serial_index += data_array[i].digit;
    }

    if (ret == SERIAL_VERIFY_SUCCESS)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }

    return;
}

void serial_view(serial_desc *desc)
{
    int i;
    serial_data *data_array = desc->serial_data_array;
    char serial[SERIAL_LENGTH_MAX];
    int serial_index;
    char buf[SERIAL_LENGTH_MAX];
    int total_len;
    
    memset(serial,0,SERIAL_LENGTH_MAX);
    printf("insert serial: ");
    scanf("%s", serial);

    my_decode(serial, strnlen(serial, SERIAL_LENGTH_MAX));

    total_len = 0;
    for (i = 0; data_array[i].name != NULL; i++)
    {
        total_len += data_array[i].digit;
    }

    if (total_len != strnlen(serial, SERIAL_LENGTH_MAX))
    {
        printf("Serial length error: input %d-digit serial\n", total_len);
        return;
    }
    
    serial_index = 0;
    for (i = 0; data_array[i].name != NULL; i++)
    {
        strncpy(buf, &serial[serial_index], data_array[i].digit);
        buf[data_array[i].digit] = '\0';
        
        printf("serial data [%s(%d)]=%s\n",
               data_array[i].name,
               data_array[i].digit,
               buf);

        serial_index += data_array[i].digit;
    }

    return;
}

