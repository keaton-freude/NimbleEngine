#pragma once

#ifdef _MSC_VER
// msvc best guess
#define NIMBLE_INLINE __forceinline
#define NIMBLE_NOINLINE __declspec(noinline)

#ifndef NIMBLE_LIKELY
#define NIMBLE_LIKELY(x) x
#endif

#ifndef NIMBLE_UNLIKELY
#define NIMBLE_UNLIKELY(x) x
#endif

#else
// gcc/clang best guess
#define NIMBLE_INLINE __attribute__((always_inline))
#define NIMBLE_NOINLINE __attribute__((noinline))

#ifndef NIMBLE_LIKELY
#define NIMBLE_LIKELY __builtin_expect(!!(x), 1)
#endif

#ifndef NIMBLE_UNLIKELY
#define NIMBLE_UNLIKELY __builtin_expect(!!(x), 0)
#endif

#endif