#if !defined(_SERIAL_H_)
#define _SERIAL_H_

#include <stdio.h>
#include <string.h>

/**
 * License descriptor table must be ended with SERIAL_SENTINEL
 */
#define SERIAL_SENTINEL {NULL, 0, NULL, NULL}

#define SERIAL_LENGTH_MAX 128
//#define SERIAL_DATA_DELIMITER    "-"

#define SERIAL_VERIFY_SUCCESS 1
#define SERIAL_VERIFY_FAIL 0


#define COMPARE_STRING_DATA(INPUT, TARGET, DIGIT)                             \
    ((strncmp(INPUT,TARGET,DIGIT) == 0) ? SERIAL_VERIFY_SUCCESS : SERIAL_VERIFY_FAIL)


/**
 * function to verify each data
 */
typedef int serial_callback_verify(char *, char *, int);


typedef struct serial_desc
{
    char *product_name;
    struct serial_data *serial_data_array;
} serial_desc;


/**
 * descriptor of each data
 */
typedef struct serial_data
{
    /**
     * name as C-string
     */
    char *name;
    /**
     * maximum digit
     */
    int digit;
    /**
     * private date required by verification function
     */
    void *private;
    /**
     * verification function
     */
    serial_callback_verify *verify_func;
} serial_data;


int serial_init(serial_desc *);
void serial_final(serial_desc *);
void serial_generate(serial_desc *);
void serial_verify(serial_desc *);
void serial_view(serial_desc *);

#endif
