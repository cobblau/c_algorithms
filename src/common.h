#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>

typedef int8_t     int8_t;
typedef uint8_t    uint8_t;
typedef int16_t    int16_t;
typedef uint16_t   uint16_t;
typedef int32_t    int32_t;
typedef uint32_t   uint32_t;
typedef int32_t    int32_t;
typedef uint32_t   uint32_t;
typedef intptr_t   int_t;
typedef uintptr_t  uint_t;


#define OK     0
#define ERROR  1

/* page size of your lovely host */
#define PAGE_SIZE  4096

/* cache line size of your lovely host */
#define CACHE_LINE 64

#define ALIGN(m, n)  ((m + (n - 1)) & ~(n - 1))


#endif
