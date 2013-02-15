
/*
 * copy of stdint.h of glib
 */

#ifndef _CAOS_TYPES_H
#define _CAOS_TYPES_H	1


#if CALIB_CFG_CPU == X86_64
# define __WORDSIZE 64
#else
# define __WORDSIZE 32
#endif



/* Exact integral types.  */

/* Signed.  */
typedef signed char		int8_t;
typedef short int		int16_t;
typedef int			int32_t;
#if __WORDSIZE == 64
typedef long int		int64_t;
#else
typedef long long int		int64_t;
#endif


/* Unsigned.  */
typedef unsigned char		uint8_t;
typedef unsigned short int	uint16_t;
typedef unsigned int		uint32_t;
#if __WORDSIZE == 64
typedef unsigned long int	uint64_t;
#else
typedef unsigned long long int	uint64_t;
#endif


/* Types for `void *' pointers.  */
#if __WORDSIZE == 64
typedef long int		intptr_t;
typedef unsigned long int	uintptr_t;
#else
typedef int			intptr_t;
typedef unsigned int		uintptr_t;
#endif


/* Largest integral types.  */
#if __WORDSIZE == 64
typedef long int		intmax_t;
typedef unsigned long int	uintmax_t;
#else
typedef long long int		intmax_t;
typedef unsigned long long int	uintmax_t;
#endif


#if __WORDSIZE == 64
# define __INT64_C(c)	c ## L
# define __UINT64_C(c)	c ## UL
#else
# define __INT64_C(c)	c ## LL
# define __UINT64_C(c)	c ## ULL
#endif


/* Limits of integral types.  */

/* Minimum of signed integral types.  */
#define INT8_MIN		(-128)
#define INT16_MIN		(-32767-1)
#define INT32_MIN		(-2147483647-1)
#define INT64_MIN		(-__INT64_C(9223372036854775807)-1)
/* Maximum of signed integral types.  */
#define INT8_MAX		(127)
#define INT16_MAX		(32767)
#define INT32_MAX		(2147483647)
#define INT64_MAX		(__INT64_C(9223372036854775807))

/* Maximum of unsigned integral types.  */
#define UINT8_MAX		(255)
#define UINT16_MAX		(65535)
#define UINT32_MAX		(4294967295U)
#define UINT64_MAX		(__UINT64_C(18446744073709551615))


/* Values to test for integral types holding `void *' pointer.  */
#if __WORDSIZE == 64
# define INTPTR_MIN		(-9223372036854775807L-1)
# define INTPTR_MAX		(9223372036854775807L)
# define UINTPTR_MAX		(18446744073709551615UL)
#else
# define INTPTR_MIN		(-2147483647-1)
# define INTPTR_MAX		(2147483647)
# define UINTPTR_MAX		(4294967295U)
#endif


/* Minimum for largest signed integral type.  */
#define INTMAX_MIN		(-__INT64_C(9223372036854775807)-1)
/* Maximum for largest signed integral type.  */
#define INTMAX_MAX		(__INT64_C(9223372036854775807))

/* Maximum for largest unsigned integral type.  */
#define UINTMAX_MAX		(__UINT64_C(18446744073709551615))


/* Limit of `size_t' type.  */
#if __WORDSIZE == 64
# define SIZE_MAX		(18446744073709551615UL)
#else
# define SIZE_MAX		(4294967295U)
#endif

/* Limits of `wchar_t'.  */
#ifndef WCHAR_MIN
/* These constants might also be defined in <wchar.h>.  */
# define WCHAR_MIN		__WCHAR_MIN
# define WCHAR_MAX		__WCHAR_MAX
#endif

/* Limits of `wint_t'.  */
#define WINT_MIN		(0u)
#define WINT_MAX		(4294967295u)


/* Signed.  */
#define INT8_C(c)	c
#define INT16_C(c)	c
#define INT32_C(c)	c
#if __WORDSIZE == 64
# define INT64_C(c)	c ## L
#else
# define INT64_C(c)	c ## LL
#endif

/* Unsigned.  */
#define UINT8_C(c)	c
#define UINT16_C(c)	c
#define UINT32_C(c)	c ## U
#if __WORDSIZE == 64
# define UINT64_C(c)	c ## UL
#else
# define UINT64_C(c)	c ## ULL
#endif



#endif /* end of file */
