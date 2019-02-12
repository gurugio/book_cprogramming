#include <stdio.h>
#include <string.h>
#include "serial_interface.h"

int main(void)
{
	/* new info should be added at top */
	REGISTER_SERIALINFO(product);
	REGISTER_SERIALINFO(id);
	
	create_serial();
    
	return 0;
}
