#ifndef VARIADIC

/*
http://stackoverflow.com/questions/1472138/c-default-arguments#3037798
*/

#define _NARG2(_0, _1, _2, ...) _2
#define NUMARG2(...) _NARG2(__VA_ARGS__, 2, 1, 0)
#define _NARG3(_0, _1, _2, _3, ...) _3
#define NUMARG3(...) _NARG3(__VA_ARGS__, 3, 2, 1, 0)
#define _NARG4(_0, _1, _2, _3, _4, ...) _4
#define NUMARG4(...) _NARG4(__VA_ARGS__, 4, 3, 2, 1, 0)
#define _NARG5(_0, _1, _2, _3, _4, _5, ...) _5
#define NUMARG5(...) _NARG5(__VA_ARGS__, 5, 4, 3, 2, 1, 0)
#define _NARG6(_0, _1, _2, _3, _4, _5, _6, ...) _6
#define NUMARG6(...) _NARG6(__VA_ARGS__, 6, 5, 4, 3, 2, 1, 0)
#define _NARG7(_0, _1, _2, _3, _4, _5, _6, _7, ...) _7
#define NUMARG7(...) _NARG7(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1, 0)
#define _NARG8(_0, _1, _2, _3, _4, _5, _6, _7, _8, ...) _8
#define NUMARG8(...) _NARG8(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define _NARG9(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, ...) _9
#define NUMARG9(...) _NARG9(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define __VARIADIC(name, num_args, ...) name ## _ ## num_args (__VA_ARGS__)
#define _VARIADIC(name, num_args, ...) name (__VARIADIC(name, num_args, __VA_ARGS__))
#define VARIADIC(name, num_args, ...) _VARIADIC(name, num_args, __VA_ARGS__)
#define VARIADIC2(name, num_args, ...) __VARIADIC(name, num_args, __VA_ARGS__)

// Vary function name by number of arguments supplied
#define VARIADIC_NAME(name, num_args) name ## _ ## num_args ## _name ()
#define NVARIADIC(name, num_args, ...) _VARIADIC(VARIADIC_NAME(name, num_args), num_args, __VA_ARGS__)

#endif