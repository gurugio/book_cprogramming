
#include <stdio.h>
#include <string.h>


#define ID_LEN 8
#define PRODUCT_LEN 1


int analyze_serial(char *serial, int serial_len,
                   char *id, int id_len,
                   char *product, int product_len)
{
    int i;

    if (id_len + product_len != serial_len)
    {
        return 1;
    }
    else
    {
        for (i = 0; i < serial_len; i++)
        {
            serial[i] -= 0x10;
        }

        strncpy(product, serial, product_len);
        product[product_len] = '\0';
        
        strncpy(id, serial+product_len, id_len);
        id[id_len] = '\0';
    }

    return 0;
}

int main(void)
{
    char id[32];
    char product[32];
    char serial[64] = {0,};

    printf("Input Serial Number[9-digits]:");
    fgets(serial, 64, stdin);
    serial[strlen(serial) - 1] = '\0';
    
    analyze_serial(serial, strlen(serial),
                   id, ID_LEN,
                   product, PRODUCT_LEN);
    
    printf("id=%s product=%s\n", id, product);
    return 0;
}


