#ifndef _C11TYPES
#define _C11TYPES

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

#include <limits.h>
#define SIZEOF(x) (sizeof(x) * (CHAR_BIT/8))

#endif