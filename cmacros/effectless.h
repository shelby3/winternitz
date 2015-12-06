#ifndef _EFFECTLESS
#define EFFECTLESS

/*
Macros without multiple execution of side-effects.
*/

/*
Generic min and max without double execution of side-effects:
  http://stackoverflow.com/questions/3437404/min-and-max-in-c
*/

#define _CHOOSE(boolop, a, b, uid)                                  \
  ({                                                                \
    decltype(a) _a_ ## uid = (a);                                   \
    decltype(b) _b_ ## uid = (b);                                   \
    (_a_ ## uid) boolop (_b_ ## uid) ? (_a_ ## uid) : (_b_ ## uid); \
  })
#undef MIN
#undef MAX
#define MIN(a, b) _CHOOSE(<, a, b, __COUNTER__)
#define MAX(a, b) _CHOOSE(>, a, b, __COUNTER__)

#endif