#include <stdint.h>
#include <stdio.h>

#include "serial.h"

extern serial_desc *g_serial_data;

int main(void)
{
    serial_init(g_serial_data);

    serial_verify(g_serial_data);

    serial_final(g_serial_data);
    
    return 0;
}

