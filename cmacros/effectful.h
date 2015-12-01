#ifndef _EFFECTFUL
#define _EFFECTFUL

/*
Macros with multiple execution of side-effects.
*/

#define PADDING(len, chunk) ((len) % (chunk) ? (chunk) - (len) % (chunk) : 0)

/*
Generic min and max with double execution of side-effects
*/

#undef MIN
#undef MAX
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#endif