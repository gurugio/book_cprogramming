#ifndef __UTIL_H__
#define __UTIL_H__



#include <calib.h>


#define __ull(x) ((unsigned long long)(x))

#define MIN(a,b) (((a) < (b)) ? (a) : (b))


typedef unsigned long long u64;
typedef signed long long s64;

typedef volatile unsigned long long atomic_u64_t;
typedef volatile signed long long atomic_s64_t;

#define TRUE 1
#define FALSE 0

/** 
 * compare and swap
 * 
 * @param ptr data to swap
 * @param old comparation value
 * @param new new value
 * 
 * @return old value
 */
static inline u64 atomic_cmpxchg(volatile void *ptr,
                                 u64 old,
                                 u64 new)
{
	u64 prev;

    asm volatile("lock;cmpxchgq %1,%2"
			     : "=a"(prev)
			     : "r"(new), "m"(*(volatile long *)ptr), "0"(old)
			     : "memory");
    return prev;
}

#define ATOMIC_SUCCESS 1
#define ATOMIC_FAIL    0

static inline u64 atomic_cas(volatile void *ptr,
                                 u64 old,
                                 u64 new)
{
	u64 prev;

    asm volatile("lock;cmpxchgq %1,%2"
			     : "=a"(prev)
			     : "r"(new), "m"(*(volatile long *)ptr), "0"(old)
			     : "memory");

    if (prev == old)
        return ATOMIC_SUCCESS;
    else
        return ATOMIC_FAIL;
}

static inline u64 atomic_inc(volatile void *ptr)
{
	u64 prev;
    u64 old = *(u64 *)ptr;
    u64 new = old + 1;

    do {
        asm volatile("lock;cmpxchgq %1,%2"
                     : "=a"(prev)
                     : "r"(new), "m"(*(volatile long *)ptr), "0"(old)
                     : "memory");
    } while(prev != old);

    /* always succeed, so return old value */
    return prev;
}


static inline u64 atomic_dec(volatile void *ptr)
{
	u64 prev;
    u64 old = *(u64 *)ptr;
    u64 new = old - 1;

    do {
        asm volatile("lock;cmpxchgq %1,%2"
                     : "=a"(prev)
                     : "r"(new), "m"(*(volatile long *)ptr), "0"(old)
                     : "memory");
    } while(prev != old);

    /* always succeed, so return old value */
    return prev;
}

#endif
