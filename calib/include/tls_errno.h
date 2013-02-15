#ifndef _TLS_ERRNO_H
#define _TLS_ERRNO_H	1

#include <caos_types.h>
#include <pthread.h>


pthread_key_t caos_tls_errno;

struct tls_errno
{
    char *expr;
    char *file;
    int32_t line;
};






#endif
