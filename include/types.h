#pragma once

#define NULL (void*)0

#define uintptr_t __wk_size_t
#define size_t __wk_size_t
#define uint8_t __wk_uint8_t
#define uint16_t __wk_uint16_t
#define uint32_t __wk_uint32_t
#define uint64_t __wk_uint64_t

#define htole

#ifdef __SYS_LITTLE_ENDIAN__
    #define ltohs(x) x
    #define ltohi(x) x
    #define htols(x) x
    #define htoli(x) x

    #define btohs(x) __builtin_bswap16(x)
    #define btohi(x) __builtin_bswap32(x)
    #define htobs(x) __builtin_bswap16(x)
    #define htobi(x) __builtin_bswap32(x)
#else
    #define ltohs(x) __builtin_bswap16(x)
    #define ltohi(x) __builtin_bswap32(x)
    #define htols(x) __builtin_bswap16(x)
    #define htoli(x) __builtin_bswap32(x)

    #define btohs(x) x
    #define btohi(x) x
    #define htobs(x) x
    #define htobi(x) x
#endif

typedef __UINTPTR_TYPE__ __wk_size_t;
typedef __UINT8_TYPE__ __wk_uint8_t;
typedef __UINT16_TYPE__ __wk_uint16_t;
typedef __UINT32_TYPE__ __wk_uint32_t;
typedef __UINT64_TYPE__ __wk_uint64_t;

