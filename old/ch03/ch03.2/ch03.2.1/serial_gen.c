
#include <stdio.h>
#include <string.h>


#define ID_LEN 8
#define PRODUCT_LEN 1


int make_serial(char *serial, int serial_len,
                char *id, int id_len,
                char *product, int product_len)
{
    int i;
    
    if (id_len + product_len > serial_len)
    {
        return 1;
    }
    else
    {
        int slen;
        
        strncpy(serial, product, product_len);
        strncat(serial, id, id_len);

        slen = strlen(serial);
        for (i = 0; i < slen; i++)
        {
            serial[i] += 0x10;
        }
    }

    return 0;
}

int main(void)
{
    char id[32];
    char product[32];
    char serial[64] = {0,};
    
    printf("Input Product Number[1 or 2]:");
    fgets(product, 32, stdin);
    product[strlen(product) - 1] = '\0';

    printf("Input User ID[8-digits]:");
    fgets(id, 32, stdin);
    id[strlen(id) - 1] = '\0';

    make_serial(serial, 64,
                id, 32,
                product, 32);
    printf("Serial=%s\n", serial);
    
    return 0;
}


