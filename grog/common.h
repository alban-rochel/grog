#pragma once
//#define DEBUG_ALBAN
#define GROG_INLINE inline __attribute__((always_inline))
#define GROG_LIKELY(x)       __builtin_expect((x),1)
#define GROG_UNLIKELY(x)     __builtin_expect((x),0)

