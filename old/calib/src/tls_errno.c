

#include <caos_types.h>


int32_t tls_init(pthread_key_t *tls_key)
{
    return pthread_key_create(aKey, tls_final);
}

int32_t tls_final(pthread_key_t tls_key)
{
    return pthread_key_delete(tls_key);
}

void tls_set(pthread_key_t tls_key, const void *data)
{
    return pthread_setspecific(tls_key, data);
}

void *tls_get(pthread_key_t tls_key)
{
    return pthread_getspecific(tls_key);
}



int32_t tls_errno_init(void)
{
    return tls_init(&caos_tls_errno);
}

int32_t tls_errno_final(void)
{
    return tls_final(caos_tls_errno);
}

int32_t tls_errno_set(const char *expr,
                    const char *file_name,
                    const int32_t line_num)
{
    struct tls_errno *error;

    errno = (struct tls_errno *)malloc(sizeof(struct tls_errno));

    if (errno != NULL)
    {
        errno->expr = expr;
        errno->file = file_name;
        errno->line = line_num;
    }
    else
    {
        return -1;
    }

    return tls_set(caos_tls_errno, errno);
}

char *tls_errno_expr(struct tls_errno *aerrno)
{
    if (NULL != aerrno)
        return aerrno->expr;
    else
        return NULL;
}

char *tls_errno_filename(struct tls_errno *aerrno)
{
    if (NULL != aerrno)
        return aerrno->file;
    else
        return NULL;
}

int32_t tls_errno_linenum(struct tls_errno *aerrno)
{
    if (NULL != aerrno)
        return aerrno->line;
    else
        return -1;
}





